/**
 * TI TMS320C64x registers.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef MODULE_REGISTER_CONTROLLER_HPP_
#define MODULE_REGISTER_CONTROLLER_HPP_

#include "module.RegisterBase.hpp"

extern void* __bss__;

namespace module
{
    class RegisterController : public ::module::RegisterBase
    { 
        typedef ::module::RegisterBase  Parent;
    
    public:
    
        /** 
         * Constructor.
         */  
        RegisterController() : Parent(),
            align8_ (0xb00cffffffffb00c),      
            a0_ (0),  a1_(0),  b0_(0),  b1_(0),
            a2_ (0),  a3_(0),  b2_(0),  b3_(0),
            a4_ (0),  a5_(0),  b4_(0),  b5_(0),
            a6_ (0),  a7_(0),  b6_(0),  b7_(0),
            a8_ (0),  a9_(0),  b8_(0),  b9_(0),
            a10_(0), a11_(0), b10_(0), b11_(0),
            a12_(0), a13_(0), b12_(0), b13_(0),
            a14_(0), a15_(0), b14_(0), b15_(0),
            a16_(0), a17_(0), b16_(0), b17_(0),
            a18_(0), a19_(0), b18_(0), b19_(0),
            a20_(0), a21_(0), b20_(0), b21_(0),
            a22_(0), a23_(0), b22_(0), b23_(0),
            a24_(0), a25_(0), b24_(0), b25_(0),
            a26_(0), a27_(0), b26_(0), b27_(0),
            a28_(0), a29_(0), b28_(0), b29_(0),
            a30_(0), a31_(0), b30_(0), b31_(0),
            amr_(0), irp_(0), csr_(0), reg_(0){
            setConstruct( construct() );
        }
        
        /** 
         * Destructor.
         */    
        virtual ~RegisterController()
        {
        }
        
        /** 
         * Initializes the registers.
         *
         * Usually, this method is used for normally restoring 
         * from some interrupt service routines.
         *
         * @param stack a stack.
         * @param entry router entry point.
         * @param arg1  first router argument.
         * @param arg2  second router argument.     
         */  
        virtual void setInitialization(::api::Stack<int64>& stack, int32 entry, int32 arg1, int32 arg2)
        {
            if(!isConstructed_) return;
            CSR csr = 0;
            // Set GIE bit to zero for restoring with out crash, because
            // this value will be set while a restoring is being executed, and
            // an enabling of global interrupts is not needed case.
            csr.bit.gie = 0;
            // Set PGIE bit to one for first calling restored context with set GIE bit.
            // Simply, we need to have enabled global interrupts in a new thread.
            csr.bit.pgie = 1;
            irp_ = entry;
            b3_  = 0; // LP
            b14_ = reinterpret_cast<int32>(&__bss__);        
            b15_ = reinterpret_cast<int32>(stack.getTos());
            a4_  = arg1;
            a5_  = 0;
            b4_  = arg2;
            b5_  = 0;
            csr_ = csr.value;
        }
      
        /** 
         * Returns a pointer to a first register of registers.
         *
         * @return memory address of registers order.
         */    
        virtual void* getRegisters()
        {
            return isConstructed_ ? &a0_ : NULL;
        }
        
    private:
    
        /**
         * Constructor.
         *
         * @return true if object has been constructed successfully.     
         */    
        bool construct()
        {
            align8_ = 0xb00cffffffffb00c;
            #ifndef EOOS_DEBUG
            a0_  = a1_  = b0_  = b1_  =
            a2_  = a3_  = b2_  = b3_  =
            a4_  = a5_  = b4_  = b5_  =
            a6_  = a7_  = b6_  = b7_  =
            a8_  = a9_  = b8_  = b9_  =
            a10_ = a11_ = b10_ = b11_ =
            a12_ = a13_ = b12_ = b13_ =
            a14_ = a15_ = b14_ = b15_ =
            a16_ = a17_ = b16_ = b17_ =
            a18_ = a19_ = b18_ = b19_ =
            a20_ = a21_ = b20_ = b21_ =
            a22_ = a23_ = b22_ = b23_ =
            a24_ = a25_ = b24_ = b25_ =
            a26_ = a27_ = b26_ = b27_ =
            a28_ = a29_ = b28_ = b29_ =
            a30_ = a31_ = b30_ = b31_ =
            amr_ = irp_ = csr_ = reg_ = 0x00000000; 
            #else // !EOOS_DEBUG
            int32 id = 0x0B00C;
            a0_  = 0xa00<<20|id;  b0_  = 0xb00<<20|id;
            a1_  = 0xa01<<20|id;  b1_  = 0xb01<<20|id;
            a2_  = 0xa02<<20|id;  b2_  = 0xb02<<20|id;
            a3_  = 0xa03<<20|id;  b3_  =0x00000000;
            a4_  = 0xa04<<20|id;  b4_  = 0xb04<<20|id;
            a5_  = 0xa05<<20|id;  b5_  = 0xb05<<20|id;
            a6_  = 0xa06<<20|id;  b6_  = 0xb06<<20|id;
            a7_  = 0xa07<<20|id;  b7_  = 0xb07<<20|id;
            a8_  = 0xa08<<20|id;  b8_  = 0xb08<<20|id;
            a9_  = 0xa09<<20|id;  b9_  = 0xb09<<20|id;
            a10_ = 0xa10<<20|id;  b10_ = 0xb10<<20|id;
            a11_ = 0xa11<<20|id;  b11_ = 0xb11<<20|id;
            a12_ = 0xa12<<20|id;  b12_ = 0xb12<<20|id;
            a13_ = 0xa13<<20|id;  b13_ = 0xb13<<20|id;
            a14_ = 0xa14<<20|id;  b14_ = 0x00000000;
            a15_ = 0x00000000;    b15_ = 0x00000000;             
            a16_ = 0xa16<<20|id;  b16_ = 0xb16<<20|id;
            a17_ = 0xa17<<20|id;  b17_ = 0xb17<<20|id;
            a18_ = 0xa18<<20|id;  b18_ = 0xb18<<20|id;
            a19_ = 0xa19<<20|id;  b19_ = 0xb19<<20|id;
            a20_ = 0xa20<<20|id;  b20_ = 0xb20<<20|id;
            a21_ = 0xa21<<20|id;  b21_ = 0xb21<<20|id;
            a22_ = 0xa22<<20|id;  b22_ = 0xb22<<20|id;
            a23_ = 0xa23<<20|id;  b23_ = 0xb23<<20|id;
            a24_ = 0xa24<<20|id;  b24_ = 0xb24<<20|id;
            a25_ = 0xa25<<20|id;  b25_ = 0xb25<<20|id;
            a26_ = 0xa26<<20|id;  b26_ = 0xb26<<20|id;
            a27_ = 0xa27<<20|id;  b27_ = 0xb27<<20|id;
            a28_ = 0xa28<<20|id;  b28_ = 0xb28<<20|id;
            a29_ = 0xa29<<20|id;  b29_ = 0xb29<<20|id;
            a30_ = 0xa30<<20|id;  b30_ = 0xb30<<20|id;
            a31_ = 0xa31<<20|id;  b31_ = 0xb31<<20|id;
            amr_ = irp_ = csr_ = reg_ = 0x00000000;
            #endif // EOOS_DEBUG
            a15_ = 0x00000000;
            return true;
        }
      
        /** 
         * Copy constructor.
         *
         * @param reg reference to source object.
         * @return boolean result.
         */
        bool copy(const RegisterController& reg)
        {
            align8_ = reg.align8_;
            a0_  = reg.a0_;   b0_  = reg.a0_;
            a1_  = reg.a1_;   b1_  = reg.a1_;
            a2_  = reg.a2_;   b2_  = reg.a2_;
            a3_  = reg.a3_;   b3_  = reg.a3_;
            a4_  = reg.a4_;   b4_  = reg.a4_;
            a5_  = reg.a5_;   b5_  = reg.a5_;
            a6_  = reg.a6_;   b6_  = reg.a6_;
            a7_  = reg.a7_;   b7_  = reg.a7_;
            a8_  = reg.a8_;   b8_  = reg.a8_;
            a9_  = reg.a9_;   b9_  = reg.a9_;
            a10_ = reg.a10_;  b10_ = reg.a10_;
            a11_ = reg.a11_;  b11_ = reg.a11_;
            a12_ = reg.a12_;  b12_ = reg.a12_;
            a13_ = reg.a13_;  b13_ = reg.a13_;
            a14_ = reg.a14_;  b14_ = reg.a14_;
            a15_ = reg.a15_;  b15_ = reg.a15_;            
            a16_ = reg.a16_;  b16_ = reg.a16_;
            a17_ = reg.a17_;  b17_ = reg.a17_;
            a18_ = reg.a18_;  b18_ = reg.a18_;
            a19_ = reg.a19_;  b19_ = reg.a19_;
            a20_ = reg.a20_;  b20_ = reg.a20_;
            a21_ = reg.a21_;  b21_ = reg.a21_;
            a22_ = reg.a22_;  b22_ = reg.a22_;
            a23_ = reg.a23_;  b23_ = reg.a23_;
            a24_ = reg.a24_;  b24_ = reg.a24_;
            a25_ = reg.a25_;  b25_ = reg.a25_;
            a26_ = reg.a26_;  b26_ = reg.a26_;
            a27_ = reg.a27_;  b27_ = reg.a27_;
            a28_ = reg.a28_;  b28_ = reg.a28_;
            a29_ = reg.a29_;  b29_ = reg.a29_;
            a30_ = reg.a30_;  b30_ = reg.a30_;
            a31_ = reg.a31_;  b31_ = reg.a31_;
            amr_ = reg.amr_;
            irp_ = reg.irp_;
            csr_ = reg.csr_;
            reg_ = reg.reg_;
            return reg.isConstructed();
        }
        
        /** 
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        RegisterController(const RegisterController& obj) : //Parent(),
            align8_ (obj.align8_),
            a0_ (obj.a0_),  a1_ (obj.a1_),  b0_ (obj.b0_),  b1_ (obj.b1_),
            a2_ (obj.a2_),  a3_ (obj.a3_),  b2_ (obj.b2_),  b3_ (obj.b3_),
            a4_ (obj.a4_),  a5_ (obj.a5_),  b4_ (obj.b4_),  b5_ (obj.b5_),
            a6_ (obj.a6_),  a7_ (obj.a7_),  b6_ (obj.b6_),  b7_ (obj.b7_),
            a8_ (obj.a8_),  a9_ (obj.a9_),  b8_ (obj.b8_),  b9_ (obj.b9_),
            a10_(obj.a10_), a11_(obj.a11_), b10_(obj.b10_), b11_(obj.b11_),
            a12_(obj.a12_), a13_(obj.a13_), b12_(obj.b12_), b13_(obj.b13_),
            a14_(obj.a14_), a15_(obj.a15_), b14_(obj.b14_), b15_(obj.b15_),
            a16_(obj.a16_), a17_(obj.a17_), b16_(obj.b16_), b17_(obj.b17_),
            a18_(obj.a18_), a19_(obj.a19_), b18_(obj.b18_), b19_(obj.b19_),
            a20_(obj.a20_), a21_(obj.a21_), b20_(obj.b20_), b21_(obj.b21_),
            a22_(obj.a22_), a23_(obj.a23_), b22_(obj.b22_), b23_(obj.b23_),
            a24_(obj.a24_), a25_(obj.a25_), b24_(obj.b24_), b25_(obj.b25_),
            a26_(obj.a26_), a27_(obj.a27_), b26_(obj.b26_), b27_(obj.b27_),
            a28_(obj.a28_), a29_(obj.a29_), b28_(obj.b28_), b29_(obj.b29_),
            a30_(obj.a30_), a31_(obj.a31_), b30_(obj.b30_), b31_(obj.b31_),
            amr_(obj.amr_), irp_(obj.irp_), csr_(obj.csr_), reg_(obj.reg_){
            setConstruct( copy(obj) );
        }
        
        /** 
         * Assignment operator.
         *
         * @param obj reference to source object.
         * @return reference to this object.       
         */     
        RegisterController& operator =(const RegisterController& obj)
        {
          //Parent::operator=(obj);
            copy(obj);
            return *this;
        }       
        
        /**
         * Control Status Register (CSR).
         */
        union CSR
        {
            CSR() : 
                value (0){
            }
            
            CSR(uint32 def) : 
                value (def){
            }
            
            CSR(const CSR& reg) : 
                value (reg.value){
            }        
            
           ~CSR()
            {
                value = 0;
            }    
           
            CSR& operator =(const CSR& reg)
            {
                value = reg.value;
                return *this;
            }
          
            uint32 value;
            struct 
            {
                uint32 gie        : 1;
                uint32 pgie       : 1;
                uint32 dcc        : 3;
                uint32 pcc        : 3;
                uint32 en         : 1;
                uint32 sat        : 1;
                uint32 pwrd       : 6;
                uint32 revisionId : 8;
                uint32 cpuId      : 8;
            } bit;
        };
      
        /**
         * For alignment to eight on stack
         */
        int64 align8_;
      
        /**
         * CPU registers.    
         */
        int32 a0_,  a1_,  b0_,  b1_,
              a2_,  a3_,  b2_,  b3_,
              a4_,  a5_,  b4_,  b5_,
              a6_,  a7_,  b6_,  b7_,
              a8_,  a9_,  b8_,  b9_,
              a10_, a11_, b10_, b11_,
              a12_, a13_, b12_, b13_,
              a14_, a15_, b14_, b15_,
              a16_, a17_, b16_, b17_,
              a18_, a19_, b18_, b19_,
              a20_, a21_, b20_, b21_,
              a22_, a23_, b22_, b23_,
              a24_, a25_, b24_, b25_,
              a26_, a27_, b26_, b27_,
              a28_, a29_, b28_, b29_,
              a30_, a31_, b30_, b31_,
              amr_, irp_, csr_, reg_; 
      
    };  
}
#endif // MODULE_REGISTER_CONTROLLER_HPP_
