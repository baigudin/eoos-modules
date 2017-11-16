/** 
 * Boot routine.
 *
 * The module performs the tasks to initialize C/C++ run-time environment.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#include "module.Boot.hpp"

namespace module
{
    /**
     * Initialization.
     *
     * @return true if no errors.
     */
    bool Boot::initialize()
    {
        // Initialize global and static variables
        CRecord* cRecord = getCRecord();
        while(cRecord != NULL)
        {
            cell size = cRecord->size > 0 ? cRecord->size : 0 - cRecord->size;
            cell* addr = reinterpret_cast<cell*>(cRecord->addr[1] | cRecord->addr[0]);
            for(int32 i=0; i<size; i++) 
                addr[i] = cRecord->data[i];
            cRecord = getCRecord(cRecord);
        }
        // Call global C++ class default constructors
        PRecord* pRecord = getPRecord();
        while(pRecord != NULL)
        {
            pRecord->constructor();
            pRecord = getPRecord(pRecord);
        }    
        return true;   
    }
    
    /**
     * Deinitialization.
     */
    void Boot::deinitialize()
    {
    }
    
    /**
     * Returns the next record adderess.
     * 
     * @param record the record address for getting next record after this.
     * @return memory address of the next record or NULL.
     */
    Boot::CRecord* Boot::getCRecord(Boot::CRecord* record)
    {
        cell size = record->size > 0 ? record->size : 0 - record->size;
        record = reinterpret_cast<CRecord*>(&record->data[size]);
        return size != 0 ? record : NULL;
    }
  
    /**
     * Returns the next record adderess.
     * 
     * @param record the record address for getting next record after this.
     * @return memory address of the next record or NULL.
     */
    Boot::PRecord* Boot::getPRecord(Boot::PRecord* record)
    {
        record = record + 1;
        return record->constructor != NULL ? record : NULL;
    }  
}
