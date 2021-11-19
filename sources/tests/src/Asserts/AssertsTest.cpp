#include "gtest/gtest.h"
#include "Asserts/AssertMacros.h"

struct AssertionData
{
	AssertionData() = default;
	AssertionData(std::string message, int line, const char * file)
		: message(std::move(message))
		, line(line)
		, file(file)
	{
	}
	
	std::string		message;
	int				line = 0;
	const char *	file = nullptr;
};

class TestAssertFixture : public testing::Test
{
public:
	TestAssertFixture()
	{
	}
	
	void SetUp() override
	{
		_owner = std::make_shared<int>(42);
		std::weak_ptr<void> weakOwner = _owner;
		Core::SetAssertionHandler([this, weakOwner](const std::string & message, int line, const char * file)
		{
			if (weakOwner.expired())
			{
				return;
			}
			HandleAssertion(message, line, file);
		});
	}

	void TearDown() override
	{
		_assertionData.clear();
		_owner.reset();
		Core::SetAssertionHandler(nullptr);
	}

	const AssertionData * GetLastAssertionData() const
	{
		if (_assertionData.empty())
		{
			return nullptr;
		}
		return &_assertionData.back();
	}

	const auto & GetAllAssertionData() const
	{
		return _assertionData;
	}
	
	void VerifyAssertFailed(std::optional<std::string> exactlyMessage = std::nullopt)
	{
		auto * data = GetLastAssertionData();
		ASSERT_NE(data, nullptr);
		if (exactlyMessage)
		{
			EXPECT_EQ(data->message, *exactlyMessage);
		}
		else
		{
			EXPECT_EQ(data->message.empty(), false);
		}
		EXPECT_NE(data->file, nullptr);
		EXPECT_NE(data->line, 0);
	}

	void VerifyAssertAccepted()
	{
		EXPECT_EQ(GetLastAssertionData(), nullptr);
	}

private:
	void HandleAssertion(const std::string & message, int line, const char * file)
	{
		_assertionData.emplace_back(message, line, file);
	}

	std::vector<AssertionData>		_assertionData;
	std::shared_ptr<void>			_owner;
};

#pragma warning(push)
#pragma warning(disable: 4127)

TEST_F(TestAssertFixture, CheckTrueAssert)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	Assert(2 + 2 == 4);
	VerifyAssertAccepted();
}

TEST_F(TestAssertFixture, CheckFalseAssert)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	Assert(2 + 2 == 5);
	VerifyAssertFailed();
}

TEST_F(TestAssertFixture, CheckTrueAssertWithMessage)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	Assert2(2 + 2 == 4, "Test message: assetion failed.");
	VerifyAssertAccepted();
}

TEST_F(TestAssertFixture, CheckFalseAssertWithMessage)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	Assert2(2 + 2 == 5, "Test message: assetion failed.");
	VerifyAssertFailed("Test message: assetion failed.");
}

#pragma warning(pop)

TEST_F(TestAssertFixture, CheckTrueVerify)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_TRUE(Verify(2 + 2 == 4));
	VerifyAssertAccepted();
}

TEST_F(TestAssertFixture, CheckFalseVerify)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_FALSE(Verify(2 + 2 == 5));
	VerifyAssertFailed();
}

TEST_F(TestAssertFixture, CheckTrueVerifyWithMessage)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_TRUE(Verify2(2 + 2 == 4, "Test verify error"));
	VerifyAssertAccepted();
}

TEST_F(TestAssertFixture, CheckFalseVerifyWithMessage)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_FALSE(Verify2(2 + 2 == 5, "Test verify error"));
	VerifyAssertFailed("Test verify error");
}

TEST_F(TestAssertFixture, CheckFalseVerifyWithMessageWithNegation)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_TRUE(!Verify2(2 + 2 == 5, "Test verify error"));
	VerifyAssertFailed("Test verify error");
}

TEST_F(TestAssertFixture, CheckTrueVerifyWithMessageWithNegation)
{
	ASSERT_EQ(GetLastAssertionData(), nullptr);
	EXPECT_FALSE(!Verify2(2 + 2 == 4, "Test verify error"));
	VerifyAssertAccepted();
}