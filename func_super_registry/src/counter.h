/*
 * counter.h
 *
 *  Created on: Jun 19, 2024
 *      Author: mortenl
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include <iostream>
#include <functional>
#include <map>
#include <string>


/**************************************** Counter ****************************************/

class Counter
{
    private:
    	int _count = 0;
    public:
		Counter(int count = 0) : _count(count) {}
		void Increment(void) { ++_count; }
		void Decrement(void) { --_count; }
		int Get(void) { return(_count); }
		void Set(int val) { _count = val; }
};


// Simple wrappers
static void DoReset(Counter *cnt) { cnt->Set(0); }
static void DoIncrement(Counter *cnt) { cnt->Increment(); }
static void DoDecrement(Counter *cnt) { cnt->Decrement(); }
static void DoGet(Counter *cnt) { int val = cnt->Get(); std::cout << "Value = " << val << std::endl; }
//static void DoSet(Counter *cnt) { cnt->Set(); }


class CounterRegistry
{
    private:
    	std::map<std::string, Counter*> _counters;
    	std::map<std::string, std::function<void(Counter*)>> _funcs;
    public:
    	bool CounterFound(const std::string name)
		{
			return( _counters.find(name) != _counters.end() );
		}

		bool FuncFound(const std::string func)
		{
			return( _funcs.find(func) != _funcs.end() );
		}

    	CounterRegistry()
    	{
    		// RST
    		std::function<void(Counter*)> rst_func = &DoReset;
    		_funcs["rst"] = rst_func;
    		// INC
    		std::function<void(Counter*)> inc_func = &DoIncrement;
			_funcs["inc"] = inc_func;
			// DEC
			std::function<void(Counter*)> dec_func = &DoDecrement;
			_funcs["dec"] = dec_func;
			// GET
			std::function<void(Counter*)> get_func = &DoGet;
			_funcs["get"] = get_func;
    	}

    	void AddCounter(std::string name)
    	{
    		auto counter = new Counter();

    		_counters[name] = counter;
    	}

    	void Executor(std::string name, std::string func)
    	{
    		if ( CounterFound(name) )
    		{
    			if ( FuncFound(func) )
				{
					// Execute:
					_funcs[ func ](_counters[name]);
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


#endif /* COUNTER_H_ */
