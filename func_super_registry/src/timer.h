/*
 * timer.h
 *
 *  Created on: Jun 19, 2024
 *      Author: mortenl
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <functional>
#include <map>
#include <string>


/**************************************** TIMER ****************************************/

class Timer
{
    private:
    	int _timeout = 0;
    	int _elapsed = 0;
    	bool _running = false;
    public:
		Timer(int timeout = 0) : _timeout(timeout) {}
		void Reset(void) { _elapsed = 0; }
		void Start(void) { _running = true; }
		void Stop(void) { _running = false; }
		int GetElapsed(void) { return(_elapsed); }
};


// Simple wrappers
static void DoStart(Timer *tmr) { tmr->Start(); }
static void DoClear(Timer *tmr) { tmr->Reset(); }
static void DoStop(Timer *tmr) { tmr->Stop(); }
static void DoShow(Timer *tmr) { int val = tmr->GetElapsed(); std::cout << "Elapsed = " << val << std::endl; }


class TimerRegistry
{
    private:
    	std::map<std::string, Timer*> _timers;
    	std::map<std::string, std::function<void(Timer*)>> _funcs;

    public:
    	bool TimerFound(const std::string name)
		{
			return( _timers.find(name) != _timers.end() );
		}

		bool FuncFound(const std::string func)
		{
			return( _funcs.find(func) != _funcs.end() );
		}

    	TimerRegistry()
    	{
    		// RST
    		std::function<void(Timer*)> rst_func = &DoClear;
    		_funcs["rst"] = rst_func;
    		// INC
    		std::function<void(Timer*)> start_func = &DoStart;
			_funcs["start"] = start_func;
			// DEC
			std::function<void(Timer*)> stop_func = &DoStop;
			_funcs["stop"] = stop_func;
			// GET
			std::function<void(Timer*)> get_func = &DoShow;
			_funcs["get"] = get_func;
    	}

    	void AddTimer(std::string name)
    	{
    		auto timer = new Timer();

    		_timers[name] = timer;
    	}

    	void Executor(std::string name, std::string func)
    	{
    		if (TimerFound(name) )
    		{
    			if ( FuncFound(func) )
				{
					// Execute:
					_funcs[ func ](_timers[name]);
				}
    			else
    			{
    				std::cout << "Could not find function named: " << func << std::endl;
    			}
    		}
    		else
    		{
    			std::cout << "Could not find counter named: " << name << std::endl;
    		}

    	}

};




#endif /* TIMER_H_ */
