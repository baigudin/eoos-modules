/** 
 * Hardware interrupt controller.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#include "module.InterruptController.hpp"

namespace module
{
    /**
     * HW interrupt handle routing.
     *
     * @param index index of HW interrupt vector number in contexts table
     */  
    void InterruptController::handler(register int32 index)
    {
        register ContextHi* ctx = &contextHi_[index];
        #ifdef EOOS_NESTED_INT
        register bool is = ctx->disable();
        Interrupt::enableAll(true);
        #endif
        ctx->handler->main();
        #ifdef EOOS_NESTED_INT
        Interrupt::disableAll();
        ctx->enable(is);
        #endif
    }
     
    /**
     * The module has been initialized successfully (no boot).
     */
    int32 InterruptController::isInitialized_;  
  
    /**
     * HW interrupt registers (no boot).
     */
    reg::Intc* InterruptController::regInt_;  
    
    /**
     * All interrupt resource contexts (no boot).
     */
    InterruptController::Contexts* InterruptController::contexts_;
    
    /**
     * All interrupt resource contexts (no boot).
     */    
    InterruptController::ContextHi* InterruptController::contextHi_;
    
    /**
     * Buffer for allocating low level interrupts contexts table (no boot).
     */    
    uint64 InterruptController::Contexts::buffer_[ InterruptController::Contexts::NUMBER_VECTORS ];      
  
}

