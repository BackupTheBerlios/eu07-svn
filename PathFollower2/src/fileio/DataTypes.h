#ifndef SPT_READWRITE
#define SPT_READWRITE 1

namespace spt {

// Size of datatypes
#define BOOLSIZE	1
#define CHARSIZE	1
#define SHORTSIZE	2
#define INTSIZE		4
#define FLOATSIZE	4
#define LONGSIZE	4
#define DOUBLESIZE	8

// Objects and Nodes
#define SPTOBJECT                       0x00000001
#define SPTMOVEMENTPATH			0x00000002
#define SPTMOVER			0x00000101
#define SPTDLLMOVER			0x00000102
#define SPTTRAINSET			0x00000103

//#define IVENODE                         0x00000002

}
#endif // SPT_READWRITE
