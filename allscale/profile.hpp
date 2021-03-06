#ifndef ALLSCALE_PROFILE_HPP
#define ALLSCALE_PROFILE_HPP


#include <chrono>
#include <math.h>

namespace allscale
{
    struct profile 
    {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point end;
        std::chrono::steady_clock::time_point result_ready;
#if HAVE_PAPI
        long long papi_counters_start[8];
        long long papi_counters_stop[8];
#endif

        // Data to compute mean and stdev for all children
        // see http://www.johndcook.com/standard_deviation.html
        // or Donald Knuth's Art of Computer Programming, Vol 2, page 232, 3rd edition
        unsigned int m_n;
        double m_oldM, m_newM, m_oldS, m_newS;


        profile():start(std::chrono::steady_clock::now())
        {
           m_n = 0;
           m_oldM = m_newM = m_oldS = m_newS = 0.0;

        }


	void push(double x)
	{
	   m_n++;

	   // See Knuth TAOCP vol 2, 3rd edition, page 232
	   if (m_n == 1)
	   {
		m_oldM = x;
		m_oldS = 0.0;
	   }
	   else
	   {
		m_newM = m_oldM + (x - m_oldM)/m_n;
		m_newS = m_oldS + (x - m_oldM)*(x - m_newM);

		// set up for next iteration
		m_oldM = m_newM; 
		m_oldS = m_newS;
	   }
	}

	double Mean() const
	{
	   return (m_n > 0) ? m_newM : 0.0;
	}

	double Variance() const
	{
	   return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 );
	}

	double StandardDeviation() const
	{
	   return sqrt( Variance() );
	}


        // Other performance metrics
        double get_exclusive_time() {

           std::chrono::duration<double> time_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
           return time_elapsed.count();

        }


        double get_inclusive_time() {

           std::chrono::duration<double> time_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(result_ready - start);
           return time_elapsed.count();

        }

    };



}

#endif
