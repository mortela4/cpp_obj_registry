/*
 * adc.h
 *
 *  Created on: Jun 19, 2024
 *      Author: mortenl
 */

#ifndef ADC_H_
#define ADC_H_

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <optional>

using opt_retval_t = std::optional<std::variant<int, float>>;


/********************************* ADC *************************************/

class ADC
{
    private:
    	float _value = 0.5F;
    	bool _enabled = false;
    public:
		ADC(){}
		void Convert(void) { std::cout << "Converting ..." << std::endl; _value = (_enabled) ? _value + 0.15F : 0.0F; }
		float GetVal(void) { return(_value); }
		void Enable(void) { _enabled = true; }
		void Disable(void) { _enabled = false; }
};


// Simple wrappers
static opt_retval_t DoConvert(ADC *adc) { adc->Convert(); return(std::nullopt); }
static opt_retval_t DoEnable(ADC *adc) { adc->Enable(); return(std::nullopt); }
static opt_retval_t DoDisable(ADC *adc) { adc->Disable(); return(std::nullopt); }
static opt_retval_t DoGetVal(ADC *adc)
{
	opt_retval_t val = adc->GetVal();

	return(val);
}


class AdcRegistry
{
    private:
    	std::map<std::string, ADC*> _adconverters;
    	std::map<std::string, std::function<opt_retval_t(ADC*)>> _funcs;

    public:
    	bool AdcFound(const std::string name)
		{
			return( _adconverters.find(name) != _adconverters.end() );
		}

		bool FuncFound(const std::string func)
		{
			return( _funcs.find(func) != _funcs.end() );
		}

    	AdcRegistry()
    	{
    		// RST
    		std::function<opt_retval_t(ADC*)> rst_func = &DoConvert;
    		_funcs["conv"] = rst_func;
    		// INC
    		std::function<opt_retval_t(ADC*)> enable_func = &DoEnable;
			_funcs["enable"] = enable_func;
			// DEC
			std::function<opt_retval_t(ADC*)> disable_func = &DoDisable;
			_funcs["disable"] = disable_func;
			// GET
			std::function<opt_retval_t(ADC*)> get_func = &DoGetVal;
			_funcs["get"] = get_func;
    	}

    	void AddAdc(std::string name)
    	{
    		auto adc = new ADC();

    		_adconverters[name] = adc;
    	}

    	opt_retval_t Executor(std::string name, std::string func)
    	{
    		opt_retval_t optRetVal = std::nullopt;

    		if (AdcFound(name) )
    		{
    			if ( FuncFound(func) )
				{
					// Execute:
    				optRetVal = _funcs[ func ](_adconverters[name]);
				}
    			else
    			{
    				std::cout << "Could not find function named: " << func << std::endl;
    			}
    		}
    		else
    		{
    			std::cout << "Could not find ADC named: " << name << std::endl;
    		}

    		return(optRetVal);
    	}

};

#endif /* ADC_H_ */
