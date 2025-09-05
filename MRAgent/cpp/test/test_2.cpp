
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "ImiMrAgent.h"

#include "pd/test1.pb.h"
#include "pd/test1.pb.cc"

using namespace std;
using namespace imimr;

void serialize_process(string& x) {
	cout << "serialize_process" << endl;
	tutorial::Person person;
	person.set_name("Obama");
	person.set_id(1234);
	person.set_email("1234@qq.com");

	tutorial::Person::PhoneNumber *phone1 = person.add_phones();
	phone1->set_number("110");
	phone1->set_type(tutorial::Person::MOBILE);

	tutorial::Person::PhoneNumber *phone2 = person.add_phones();
	phone2->set_number("119");
	phone2->set_type(tutorial::Person::HOME);

	// ofstream output("person_file", ios::out | ios::trunc | ios::binary);

    // ::google::protobuf::io::OstreamOutputStream oo(&cout, 256);

    
    // person.SerializeWithCachedSizes()

	// if( !person.se(&output) ) {
	// 	cout << "Fail to SerializeToOstream." << endl;
	// }
    int len = person.ByteSizeLong();
	cout << "person.ByteSizeLong() : " << len << endl;

    // string x;
    person.SerializeToString(&x);

    


}

void parse_process(string& x) {
	cout << "parse_process" << endl;
	tutorial::Person result;


	// fstream input("person_file", ios::in | ios::binary);

    if(!result.ParseFromString(x)) {
		cout << "Fail to ParseFromIstream." << endl;
	}

	// if(!result.MergePartialFromCodedStream(buff)) {
	// 	cout << "Fail to ParseFromIstream." << endl;
	// }

	cout << result.name() << endl;
	cout << result.id() << endl;
	cout << result.email() << endl;
	for(int i = 0; i < result.phones_size(); ++i) {
		const tutorial::Person::PhoneNumber &person_phone = result.phones(i);

		switch(person_phone.type()) {
			case tutorial::Person::MOBILE :
				cout << "MOBILE phone : ";
				break;
			case tutorial::Person::HOME :
				cout << "HOME phone : ";
				break;
			case tutorial::Person::WORK :
				cout << "WORK phone : ";
				break;
			default:
				cout << "phone type err." << endl;
		}
		cout << person_phone.number() << endl;
	}
}


int main(int argc, char** argv){

    char buff[256];
    string x;
    serialize_process(x);

    parse_process(x);


    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}


