
#ifndef ALLSCALE_RUNTIME_HPP
#define ALLSCALE_RUNTIME_HPP

/**
 * The header defining the interface to be utilzed by AllScale runtime applications.
 * In particular, this is the interface utilized by the compiler when generating applications
 * targeting the AllScale runtime.
 */


#include <allscale/no_split.hpp>
#include <allscale/spawn.hpp>
#include <allscale/work_item_description.hpp>
#include <allscale/components/monitor.hpp>
#include <allscale/do_serialization.hpp>
#include <allscale/no_serialization.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/util/invoke_fused.hpp>

namespace allscale {
namespace runtime {

/**
 * A wrapper for the main function of an applicaiton handling the startup and
 * shutdown procedure as well as lunching the first work item.
 */
template<typename MainWorkItem, typename ... Args>
int main_wrapper(const Args& ... args) {
    // include monitoring support
    allscale::components::monitor_component_init();
    std::cout<<"wrapper started" << std::endl;
    // start allscale scheduler ...
    allscale::scheduler::run(hpx::get_locality_id());

    // trigger first work item on first node
    int res = 0;
    if (hpx::get_locality_id() == 0) {

        res = allscale::spawn_first<MainWorkItem>(args...).get_result();
        allscale::scheduler::stop();
    }

    // return result (only id == 0 will actually)
    return res;
}

// A class aggregating task dependencies
class dependencies {};

template<typename ... TaskRefs>
dependencies after(const TaskRefs& ... ) {
	// TODO: implement this!
	return {};
}


// ---- insieme lambda wrapper ----

template<typename ClosureType>
struct insieme_lambda_wrapper {
    ClosureType* closure;
    template<typename ... Args>
    auto operator()(const Args& ... args) {
        return closure->call(closure,args...);
    }
};

template<typename ClosureType>
insieme_lambda_wrapper<ClosureType> make_insieme_lambda_wrapper(ClosureType* cls) {
    return insieme_lambda_wrapper<ClosureType>{cls};
}


// ---- a prec operation wrapper ----

template<typename A, typename R, typename Impl>
struct prec_operation {

    Impl impl;

    treeture<R> operator()(const A& a) {
        return impl(dependencies(),a);
    }

    treeture<R> operator()(const dependencies& d, const A& a) {
        return impl(d,a);
    }

};

template<typename A, typename B, typename Impl>
prec_operation<A,B,Impl> make_prec_operation(const Impl& impl) {
    return prec_operation<A,B,Impl>{impl};
}

// ---- a generic treeture combine operator ----

struct combine_name {
    static const char *name() {
        return "combine";
    }
};

template<typename Result>
struct combine_operation {

    static constexpr bool valid = true;
    using result_type = Result;

    // Just perform the addition, no extra tasks are spawned
    template <typename Closure>
    static allscale::treeture<result_type> execute(Closure const& closure) {
        return
            allscale::treeture<result_type>{
                hpx::util::invoke(hpx::util::get<2>(closure), hpx::util::get<0>(closure), hpx::util::get<1>(closure))
            };
    }
};

template<typename Result>
using combine =
    allscale::work_item_description<
        Result,
        combine_name,
        allscale::no_serialization,
        allscale::no_split<Result>,
        combine_operation<Result>
    >;

template<typename A, typename B, typename Op, typename R = std::result_of_t<Op(A,B)>>
allscale::treeture<R> treeture_combine(const dependencies&, allscale::treeture<A>&& a, allscale::treeture<B>&& b, Op op) {
    return allscale::spawn<combine<R>>(std::move(a),std::move(b), op);
}

template<typename A, typename B, typename Op, typename R = std::result_of_t<Op(A,B)>>
allscale::treeture<R> treeture_combine(allscale::treeture<A>&& a, allscale::treeture<B>&& b, Op op) {
    return treeture_combine(after(),std::move(a),std::move(b),op);
}

allscale::treeture<void> treeture_combine(const dependencies&, allscale::treeture<void>&& a, allscale::treeture<void>&& b) {
    return allscale::treeture<void>(hpx::when_all(a.get_future(), b.get_future()));
}

allscale::treeture<void> treeture_combine(allscale::treeture<void>&& a, allscale::treeture<void>&& b) {
    return treeture_combine(after(),std::move(a),std::move(b));
}


template<typename A, typename B>
allscale::treeture<void> treeture_parallel(const dependencies&, allscale::treeture<A>&& a, allscale::treeture<B>&& b) {
	return allscale::treeture<void>(hpx::when_all(a.get_future(), b.get_future()));
}

template<typename A, typename B>
allscale::treeture<void> treeture_parallel(allscale::treeture<A>&& a, allscale::treeture<B>&& b) {
	return treeture_parallel(std::move(a),std::move(b));
}


template<typename A, typename B>
allscale::treeture<void> treeture_sequential(const dependencies&, allscale::treeture<A>&& a, allscale::treeture<B>&& b) {
	assert(false && "Not implemented!");
	return treeture_parallel(a,b);
}

template<typename A, typename B>
allscale::treeture<void> treeture_sequential(allscale::treeture<A>&& a, allscale::treeture<B>&& b) {
	return treeture_sequential(std::move(a),std::move(b));
}


} // end namespace runtime
} // end namespace allscale

#endif
