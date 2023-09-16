#include<iostream>
#include<gtest/gtest.h>
#include <gmock/gmock.h>
//==============================================================================================================================
class IDBConnection{
public:
	IDBConnection() {};
	virtual ~IDBConnection() {};
	virtual auto open() -> bool = 0;
	virtual auto ExecQuery(const char*, std::vector<std::string>&) -> bool = 0;
	virtual auto close() -> bool = 0;
};
//------------------------------------------------------------------------------------------------------------------------------
class DBConnection :public IDBConnection {
public:
	DBConnection() {};
	virtual ~DBConnection() {};
	auto open() -> bool { return true; };
	auto ExecQuery(const char* q, std::vector<std::string>& v) -> bool { return true; };
	auto close() -> bool { return false; };
};
//------------------------------------------------------------------------------------------------------------------------------
class MockConnection :public IDBConnection{ //IDBConnection {
public:
	MOCK_METHOD(bool, open, (), (override));
	MOCK_METHOD(bool, ExecQuery, (const char* instr, std::vector<std::string>& v), (override));
	MOCK_METHOD(bool, close, (), (override));
};
//------------------------------------------------------------------------------------------------------------------------------
class DBConnectionTestSuite:public ::testing::Test {
protected:
	void SetUp() {
		//std::cout << "local setup" << std::endl;
		pObj = new DBConnection;

	}
	void TearDown() {
		//std::cout << "local tear-down" << std::endl;
		delete pObj;

	}
protected:
	IDBConnection* pObj;

};
//------------------------------------------------------------------------------------------------------------------------------
TEST_F(DBConnectionTestSuite, test_valid_ptr) {
	ASSERT_NE(pObj, nullptr);
}
//------------------------------------------------------------------------------------------------------------------------------
TEST_F(DBConnectionTestSuite, test_open) {
	ASSERT_EQ(pObj->open(), true);
}
//------------------------------------------------------------------------------------------------------------------------------
TEST_F(DBConnectionTestSuite, test_close) {
	ASSERT_EQ(pObj->close(), true);
}
//------------------------------------------------------------------------------------------------------------------------------
void methods_test(IDBConnection* conn) {
	bool res{ false };
	const char instr[] = { "select * from some_table" };
	std::vector<std::string> v;
	res = conn->open();
	res = conn->ExecQuery(instr,v);
	res = conn->close();
}
//------------------------------------------------------------------------------------------------------------------------------
TEST_F(DBConnectionTestSuite,methods) {
	MockConnection mObj;
	EXPECT_CALL(mObj, open()).WillOnce(::testing::Return(true));
	EXPECT_CALL(mObj, ExecQuery).Times(::testing::AtLeast(1));
	EXPECT_CALL(mObj, close()).WillOnce(::testing::Return(true));
	methods_test(&mObj);
}
//------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS(); 


}
