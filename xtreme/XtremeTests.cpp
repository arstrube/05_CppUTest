/*
 * Copyright (c) 2010, Arnd R. Strube
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"

/// CommandLineTestRunnerTest.cpp //////////////////////////////////////////////

#ifdef HadTotakeItOutFromCommandLineTestRunnerTest

struct FakeOutput
{
    static PlatformSpecificFile fopen_fake(const char*, const char*)
    {
        return (PlatformSpecificFile)0;
    }
    static void fputs_fake(const char* str, PlatformSpecificFile)
    {
        file += str;
    }
    static void fclose_fake(PlatformSpecificFile)
    {
    }
    static int putchar_fake(int c)
    {
        console += StringFrom((char)c);
        return c;
    }
    static SimpleString file;
    static SimpleString console;
};

SimpleString FakeOutput::console = "";
SimpleString FakeOutput::file = "";

struct FakeOutputInstaller
{
    FakeOutputInstaller() : SaveFOpen(PlatformSpecificFOpen), SaveFPuts(PlatformSpecificFPuts),
        SaveFClose(PlatformSpecificFClose), SavePutchar(PlatformSpecificPutchar)
    {
        PlatformSpecificFOpen = FakeOutput::fopen_fake;
        PlatformSpecificFPuts = FakeOutput::fputs_fake;
        PlatformSpecificFClose = FakeOutput::fclose_fake;
        PlatformSpecificPutchar = FakeOutput::putchar_fake;
    }
    ~FakeOutputInstaller()
    {
        PlatformSpecificPutchar = SavePutchar; /* Must be restored immediately */
        PlatformSpecificFOpen = SaveFOpen;
        PlatformSpecificFPuts = SaveFPuts;
        PlatformSpecificFClose = SaveFClose;
    }
private:
    PlatformSpecificFile (*SaveFOpen)(const char*, const char*);
    void (*SaveFPuts)(const char*, PlatformSpecificFile);
    void (*SaveFClose)(PlatformSpecificFile);
    int (*SavePutchar)(int);
};

TEST_GROUP(CommandLineTestRunnerX)
{
    TestRegistry registry;
    UtestShell *oneTest_;

    void setup()
    {
      oneTest_ = new UtestShell("group", "test", "file", 1);
    }
    void teardown()
    {
      delete oneTest_;
    }
};

TEST(CommandLineTestRunnerX, realJunitOutputShouldBeCreatedAndWorkProperly)
{
    const char* argv[] = { "tests.exe", "-ojunit", "-v", "-kpackage", };

    FakeOutputInstaller* fakeOutput = new FakeOutputInstaller; /* UT_PTR_SET() is not reentrant */

    CommandLineTestRunner commandLineTestRunner(4, argv, &registry);
    commandLineTestRunner.runAllTestsMain();

    delete fakeOutput;

    STRCMP_CONTAINS("<testcase classname=\"package.group\" name=\"test\"", FakeOutput::file.asCharString());
    STRCMP_CONTAINS("TEST(group, test)", FakeOutput::console.asCharString());
}

#endif

/// UTestPlatformTest.cpp //////////////////////////////////////////////////////

#include "UtestPlatformWrapper.h"

TEST_GROUP(UtestPlatformTestX) {};

TEST(UtestPlatformTestX, PlatformSpecificGetPlatformSpecificTimeStringWorksProperly)
{
    struct tm* time = localtime_stub(NULL);
    time->tm_year = 115;
    time->tm_mon = 3;
    time->tm_mday = 29;
    time->tm_hour = 3;
    time->tm_min = 22;
    time->tm_sec = 55;

    STRCMP_EQUAL("2015-04-29T03:22:55", GetPlatformSpecificTimeString());
}

TEST(UtestPlatformTestX, FileFunctionsWorkProperly)
{
    PlatformSpecificFile file = PlatformSpecificFOpen("bla", "");
    PlatformSpecificFPuts("Hello World\n", file);
    STRCMP_EQUAL("Hello World\n", file_spy());
    PlatformSpecificFClose(file);
}

#if !defined(__MINGW32__)

int division(int a, int b)
{
    return a / b;
}

static int _divisionByZeroTestFunction()
{
    int divisionByZero =  division(1, 0);
    FAIL(StringFromFormat("Should have divided by zero. Outcome: %d", divisionByZero).asCharString());
    return divisionByZero;
}

TEST_GROUP(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcessX)
{
    TestTestingFixture fixture;
};

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcessX, DivisionByZeroInSeparateProcessWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.setTestFunction((void(*)())_divisionByZeroTestFunction);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process - killed by signal 8");
}

#endif
