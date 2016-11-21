#include <list>
#include <string>
using namespace std;
class SynchronousRequest{
public:
	string HTTPAcceptType;
	string HTTPContentType;
	string HTTPMethod;
	string HTTPResourceQualifyingURI;
	string XSDschemaName;
	SynchronousRequest() :
			HTTPAcceptType(""), HTTPContentType(
					""), HTTPMethod("GET"), HTTPResourceQualifyingURI(""), XSDschemaName(
					"") {

	}
	virtual ~SynchronousRequest() {

	}

	virtual void dummy_SynchronousRequest() {
	}

};


