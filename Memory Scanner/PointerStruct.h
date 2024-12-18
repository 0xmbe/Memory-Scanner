#pragma once


#if _WIN64
typedef uint64_t StackPtrType;
#else
typedef uint32_t StackPtrType;
#endif

struct _POINTER {
	StackPtrType BaseAddress; 
	int BaseAddress_intRead;
	char BaseAddress_arrRead;
	StackPtrType TargetAddress; 
	
};
struct _POINTER_MAP {
	std::vector<_POINTER> PointerLevel;
};
struct _POINTER_MAP_COLLECTION {
	std::vector<_POINTER_MAP> PointerMap;
};

const char pointerScanDepth = 40;

