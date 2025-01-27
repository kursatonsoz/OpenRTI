/* -*-c++-*- OpenRTI - Copyright (C) 2009-2015 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>

#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>

#include <TestLib.h>

// Test concurrency of creation.
// That means all but one must fail.
// But exactly one must be successful.

using namespace OpenRTI;

class OPENRTI_LOCAL TestAmbassador : public RTITest::Ambassador {
public:
  TestAmbassador(const RTITest::ConstructorArgs& constructorArgs) :
    RTITest::Ambassador(constructorArgs)
  { }

  virtual bool exec()
  {
    std::auto_ptr<rti1516::RTIambassador> ambassador;
    rti1516::RTIambassadorFactory factory;
    std::vector<std::wstring> args = getArgumentList();
    ambassador = factory.createRTIambassador(args);

    // Try that several times. Ensure correct cleanup
    unsigned n = 99;
    for (unsigned i = 0; i < n; ++i) {

      // create, should work
      try {
        ambassador->createFederationExecution(getFederationExecution(), getFddFile());

        if (!getFederationBarrier()->success())
          return false;

      } catch (const rti1516::FederationExecutionAlreadyExists&) {

        if (!getFederationBarrier()->fail())
          return false;

      } catch (const rti1516::Exception& e) {
        std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      // destroy, must work
      try {
        ambassador->destroyFederationExecution(getFederationExecution());

        if (!getFederationBarrier()->success())
          return false;

      } catch (const rti1516::FederationExecutionDoesNotExist&) {

        if (!getFederationBarrier()->fail())
          return false;

      } catch (const rti1516::Exception& e) {
        std::wcout << e.what() << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    return true;
  }
};

class OPENRTI_LOCAL Test : public RTITest {
public:
  Test(int argc, const char* const argv[]) :
    RTITest(argc, argv, false)
  { }
  virtual Ambassador* createAmbassador(const ConstructorArgs& constructorArgs)
  {
    return new TestAmbassador(constructorArgs);
  }
};

int
main(int argc, char* argv[])
{
  Test test(argc, argv);
  return test.exec();
}
