//============================================================================
// Name        : func_super_registry.cpp
// Author      : ml
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "counter.h"
#include "timer.h"
#include "adc.h"

#include <iostream>
#include <list>

#include <stdio.h>


/********************************** Super-Registry *************************************/

class ObjRegistry
{
    private:
	    AdcRegistry  _adcRegistry;
		TimerRegistry _timerRegistry;
    	CounterRegistry _counterRegistry;
    	std::list<std::string> _names;

    public:
    	ObjRegistry(){}

    	bool ValidName(const std::string newName)
    	{
    		for ( const auto& objName : _names )
    		{
    			if (newName == objName)
    			{
    				std::cout << "Object name: '" << newName << "' already exists - must be unique!" << std::endl;

    				return(false);
    			}
    		}

    		return(true);
    	}

    	void AddAdc(const std::string name)
		{
			if ( ValidName(name) )
			{
				_adcRegistry.AddAdc(name);
				_names.push_back(name);
			}
		}

    	void AddTimer(const std::string name)
    	{
    		if ( ValidName(name) )
    		{
    			_timerRegistry.AddTimer(name);
    			_names.push_back(name);
    		}
    	}

    	void AddCounter(const std::string name)
		{
    		if ( ValidName(name) )
    		{
    			_counterRegistry.AddCounter(name);
    			_names.push_back(name);
    		}
		}

    	opt_retval_t Executor(const std::string name, const std::string func)
    	{
    		opt_retval_t optRetVal = std::nullopt;

    		if ( _adcRegistry.AdcFound(name) )
			{
    			optRetVal = _adcRegistry.Executor(name, func);
			}
    		else if ( _timerRegistry.TimerFound(name) )
    		{
    			_timerRegistry.Executor(name, func);
    		}
    		else if ( _counterRegistry.CounterFound(name) )
    		{
    			_counterRegistry.Executor(name, func);
    		}
    		else
    		{
    			std::cout << "Object name: '" << name << "' does NOT exist - cannot execute!" << std::endl;
    		}

    		return(optRetVal);
    	}
};


/********************************** TESTING ***************************************/

int main()
{
    auto reg = ObjRegistry();

    reg.AddCounter("a");
    reg.AddTimer("b");
    // Fail 1:
    reg.AddCounter("b");

    reg.Executor("a", "dec");
    reg.Executor("a", "dec");
    reg.Executor("a", "get");
    reg.Executor("a", "rst");
    reg.Executor("a", "get");

    reg.Executor("b", "start");
	reg.Executor("b", "stop");
	reg.Executor("b", "get");
	reg.Executor("b", "rst");
	reg.Executor("b", "get");

	// Fail 2:
	reg.Executor("c", "rst");
	// Fail 3:
	reg.Executor("b", "set");

	reg.AddAdc("c");
	reg.Executor("c", "enable");
	reg.Executor("c", "conv");
	reg.Executor("c", "conv");
	reg.Executor("c", "get");
	reg.Executor("c", "disable");
	reg.Executor("c", "convert");
	float fval = std::get<float>(reg.Executor("c", "get").value());
	std::cout << "ADC value: " << fval << "[V]" << std::endl;
	reg.Executor("c", "enable");
	reg.Executor("c", "conv");
	reg.Executor("c", "conv");
	fval = std::get<float>(reg.Executor("c", "get").value());
	std::cout << "ADC value: " << fval << "[V]" << std::endl;
	printf("Voltage: %.02f\n", fval);

	return 0;
}

