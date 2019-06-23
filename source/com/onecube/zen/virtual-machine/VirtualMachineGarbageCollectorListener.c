
void zen_VirtualMachineGarbageCollectorListener_onAfterInitialization(
    zen_GarbageCollectorListener_t* listener,
    zen_GarbageCollector_t* collector) {
    
    zen_Class_t* class0 = zen_Context_findSystemClass(zen_core_OutOfMemoryError);
    if (!zen_Context_isClear(context)) {
        ...
    }
}