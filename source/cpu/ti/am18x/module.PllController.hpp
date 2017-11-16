/** 
 * TI ARM AM18x Phase-Locked Loop Controller (PLLC).
 *
 * This is a configuration file for configuring the PLL Controller of 
 * TI AM18x chips. As an example, we based on 25 MHz rate of 
 * input crystal clock and desire setting 375 MHz for ARM core. 
 * All rates are given as arguments of the init method. 
 * In your ways, you have to pass self rates.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef MODULE_PLL_CONTROLLER_HPP_
#define MODULE_PLL_CONTROLLER_HPP_

#include "module.PllBase.hpp"
#include "module.reg.Syscfg0.hpp"
#include "module.reg.Syscfg1.hpp"
#include "module.reg.Pllc0.hpp"
#include "module.reg.Pllc1.hpp"

namespace module
{
    class PllController : public ::module::PllBase
    {
        typedef ::module::PllBase  Parent;      
        typedef reg::Syscfg0       Syscfg0;
        typedef reg::Syscfg1       Syscfg1;
        typedef reg::Pllc0         Pllc0;
        typedef reg::Pllc1         Pllc1;
      
    public:
    
        /** 
         * Constructor.
         */     
        PllController() : Parent()
        {
            setConstruct( construct() );    
        }
        
        /** 
         * Destructor.
         */
        virtual ~PllController()
        {
        }  
      
        /**
         * Initialization.
         *
         * @param config the operating system configuration.
         * @return true if no errors.
         */
        static bool initialize(const ::Configuration& config)
        {
            isInitialized_ = 0;        
            cpuClock_ = config.cpuClock;
            oscin_    = config.sourceClock;      
            syscfg0_  = NULL;
            syscfg1_  = NULL;
            pllc0_    = NULL;
            pllc1_    = NULL;
            if( not configure() ) return false;
            isInitialized_ = IS_INITIALIZED;                  
            return true;
        }
      
        /**
         * Deinitialization.
         */
        static void deinitialize()
        {
        }
      
    private:
    
        /** 
         * Constructs the object.
         *
         * @return true if object has been constructed successfully.
         */
        bool construct()
        {
            if(isInitialized_ != IS_INITIALIZED) return false;
            return true;
        }
      
        /** 
         * Configures PLL controllers.
         *
         * @return true if controllers have been configured.
         */
        static bool configure()
        {
            bool res = true;        
            syscfg0_ = new (Syscfg0::ADDRESS) Syscfg0();
            syscfg1_ = new (Syscfg1::ADDRESS) Syscfg1();
            pllc0_ = new (Pllc0::ADDRESS) Pllc0();
            pllc1_ = new (Pllc1::ADDRESS) Pllc1();
            // Unlock the SYSCFG module
            syscfg0_->kick0r.bit.kick0 = 0x83E70B13;
            syscfg0_->kick1r.bit.kick1 = 0x95A4F1E0;
            // Unlock the PLLC0 and PLLC1 modules
            syscfg0_->cfgchip0.bit.pllMasterLock = 0;
            syscfg0_->cfgchip3.bit.pll1MasterLock = 0;  
            // Configure the PLLC0 and PLLC1 modules
            res |= pll0();
            res |= pll1();  
            // Unlock the PLLC0 and PLLC1 modules
            syscfg0_->cfgchip0.bit.pllMasterLock = 1;
            syscfg0_->cfgchip3.bit.pll1MasterLock = 1;  
            // Lock the SYSCFG module
            syscfg0_->kick0r.bit.kick0 = 0x00000000;
            syscfg0_->kick1r.bit.kick1 = 0x00000000;        
            return res;
        }
        
        /** 
         * PLL0 configuration.
         */
        static bool pll0()
        {
            // Test the power-down state
            bool isPowerDown = pllc0_->pllctl.bit.pllpwrdn;
            // Set an internal oscillator (crystal) way
            if(isPowerDown) pllc0_->pllctl.bit.clkmode = 0;      
            // Calculate the values for multiplier and dividers
            int64 pllm = cpuClock_ / oscin_ - 1;
            if(pllm & ~Pllc0::PLLM_M_PLLM) return false;
            // Switch the PLL to bypass mode
            pllc0_->pllctl.bit.pllensrc = 0;
            pllc0_->pllctl.bit.extclksrc = 0;  
            pllc0_->pllctl.bit.pllen = 0;
            for(uint32 i=0; i<4; i++);
            // Set reset is asserted
            pllc0_->pllctl.bit.pllrst = 0;
            // Up the power
            if(isPowerDown) pllc0_->pllctl.bit.pllpwrdn = 0;
            // Set pre-divider value to 1: before 25 MHz, after 25 MHz
            pllc0_->prediv.bit.ratio = 0;  
            pllc0_->prediv.bit.preden = 1;
            // Set calculated multiplier value to 15: before 25 MHz, after 375 MHz
            pllc0_->pllm.bit.pllm = pllm & Pllc0::PLLM_M_PLLM;
            // Set post-divider value to 1: before 375 MHz, after 375 MHz
            pllc0_->postdiv.bit.ratio = 0;  
            pllc0_->postdiv.bit.postden = 1;
            // Set EMIFA clocking to 93.75 MHz
            syscfg0_->cfgchip3.bit.emaClksrc = 0;
            plldiv0(pllc0_->plldiv3, 4);
            // Set ARM clocking to 375 MHz
            plldiv0(pllc0_->plldiv6, 1);
            // TODO: SUPPLEMENT SOME DIVIDER SETS WHEN THOSE ARE NEEDED
            // Set reset is not asserted
            pllc0_->pllctl.bit.pllrst = 1;
            // Wait for the PLL to lock
            for(uint32 i=0; i<200; i++);
            // Switch the PLL to normal mode, not bypassed
            pllc0_->pllctl.bit.pllen = 1;
            // Set, for an example, ARM clocking for test and debug purposes
            pllc0_->ocsel.bit.ocsrc = 0x1c;
            pllc0_->oscdiv.bit.ratio = 0;
            pllc0_->oscdiv.bit.od1en = 1;
            return true;
        } 
        
        /** 
         * PLL1 configuration.
         */
        static bool pll1()
        {
            // Test the power-down state
            bool isPowerDown = pllc1_->pllctl.bit.pllpwrdn;
            // Switch the PLL to bypass mode
            pllc1_->pllctl.bit.pllensrc = 0;
            pllc1_->pllctl.bit.pllen = 0;
            for(uint32 i=0; i<4; i++);
            // Set reset is asserted
            pllc1_->pllctl.bit.pllrst = 0;
            // Up the power
            if(isPowerDown) pllc1_->pllctl.bit.pllpwrdn = 0;
            // Set calculated multiplier value to 12: before 25 MHz, after 300 MHz
            pllc1_->pllm.bit.pllm = 11;
            // Set post-divider value to 1: before 300 MHz, after 300 MHz
            pllc1_->postdiv.bit.ratio = 0;  
            pllc1_->postdiv.bit.postden = 1;
            // Set DDR clocking to 300 MHz
            plldiv1(pllc1_->plldiv1, 1);
            // Set TIMER2/3, UART1/2, McBSP, and etc clocking to 100 MHz
            syscfg0_->cfgchip3.bit.async3Clksrc = 1;
            plldiv1(pllc1_->plldiv2, 3);
            // TODO: SUPPLEMENT SOME DIVIDER SETS WHEN THOSE ARE NEEDED
            // Set reset is not asserted
            pllc1_->pllctl.bit.pllrst = 1;
            // Wait for the PLL to lock
            for(uint32 i=0; i<200; i++);
            // Switch the PLL to normal mode, not bypassed
            pllc1_->pllctl.bit.pllen = 1;        
            // Set, for an example, DDR clocking for test and debug purposes
            pllc1_->ocsel.bit.ocsrc = 0x17;
            pllc1_->oscdiv.bit.ratio = 0;
            pllc1_->oscdiv.bit.od1en = 1;
            return true;
        } 
        
        /** 
         * Configures a PLLDIV of PLL0.
         *
         * @param plldiv reference to register.
         * @param div    divider value.
         */
        static void plldiv0(volatile Pllc0::PLLDIV& plldiv, int32 div)
        {
            while(pllc0_->pllstat.bit.gostat);
            plldiv.bit.ratio = (div - 1) & Pllc0::PLLDIV_M_RATIO;
            plldiv.bit.dNen = 1;
            pllc0_->pllcmd.bit.goset = 1;
            while(pllc0_->pllstat.bit.gostat);
        }       
        
        /** 
         * Configures a PLLDIV of PLL1.
         *
         * @param plldiv reference to register.
         * @param div    divider value.
         */
        static void plldiv1(Pllc1::PLLDIV& plldiv, int32 div)
        {
            while(pllc1_->pllstat.bit.gostat);
            plldiv.bit.ratio = (div - 1) & Pllc1::PLLDIV_M_RATIO;
            plldiv.bit.dNen = 1;
            pllc1_->pllcmd.bit.goset = 1;
            while(pllc1_->pllstat.bit.gostat);
        }  
        
        /**
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        PllController(const PllController& obj);
      
        /**
         * Assignment operator.
         *
         * @param obj reference to source object.
         * @return reference to this object.     
         */
        PllController& operator =(const PllController& obj);
        
        /**
         * The module initialized falg value.
         */
        static const int32 IS_INITIALIZED = 0x23164135;
        
        /**
         * The module has been initialized successfully (no boot).
         */
        static int32 isInitialized_;    
        
        /**
         * System Configuration Module 0 registers (no boot).
         */ 
        static Syscfg0* syscfg0_;
        
        /**
         * System Configuration Module 1 registers (no boot).
         */ 
        static Syscfg1* syscfg1_;
        
        /**
         * Phase-Locked Loop Controller 0 registers (no boot).
         */ 
        static Pllc0* pllc0_;
        
        /**
         * Phase-Locked Loop Controller 1 registers (no boot).
         */             
        static Pllc1* pllc1_;
        
        /**
         * Reference clock rate in Hz (no boot).
         */      
        static int64 oscin_;
        
        /**
         * ARM clock rate in Hz (no boot).
         */      
        static int64 cpuClock_;
      
    };
    
    /**
     * The module has been initialized successfully (no boot).
     */
    int32 PllController::isInitialized_;    
    
    /**
     * System Configuration Module 0 registers (no boot).
     */ 
    reg::Syscfg0* PllController::syscfg0_;
    
    /**
     * System Configuration Module 1 registers (no boot).
     */ 
    reg::Syscfg1* PllController::syscfg1_;
    
    /**
     * Phase-Locked Loop Controller 0 registers (no boot).
     */ 
    reg::Pllc0* PllController::pllc0_;
    
    /**
     * Phase-Locked Loop Controller 1 registers (no boot).
     */             
    reg::Pllc1* PllController::pllc1_;
    
    /**
     * Reference clock rate in Hz (no boot).
     */      
    int64 PllController::oscin_;
    
    /**
     * ARM clock rate in Hz (no boot).
     */      
    int64 PllController::cpuClock_;

}
#endif // MODULE_PLL_CONTROLLER_HPP_
