#include <windows.h>

class KeepAwakeScope {
public:
	KeepAwakeScope() {
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	}
	~KeepAwakeScope() {
		SetThreadExecutionState(ES_CONTINUOUS); // restore normal behavior
	}
};