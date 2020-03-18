
// Standard Library:
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <set>
#include <list>
#include <memory>

// This project:
#include <bxfactories/bxfactories.hpp>

namespace examples {

  /// A base class which is here a pure abstract interface with a 'run' method.
  class i_runner
  {
  public:
    
    virtual ~i_runner() = default;
 
    /// The main interface method
    virtual void run() = 0;

    // We declare a system registration mechanism for all classes inherited from this base interface class
    BXFACTORIES_FACTORY_SYSTEM_REGISTER_INTERFACE(i_runner);
  
  };
  
  // We implement the system registration mechanism for all classes inherited from this base interface class
  BXFACTORIES_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(examples::i_runner,
                                                     "examples::i_runner/__system__")

  /// A concrete class inherited from the i_runner interface above
  class foo_runner
    : public i_runner
  {
  public:
     
    foo_runner()
    {
      std::cerr << "[log] foo_runner::ctor: Hello, foo!" << std::endl;
      return;
    }
    
    virtual ~foo_runner()
    {
      std::cerr << "[log] foo_runner::dtor: Bye, foo!" << std::endl;
      return;
    }
    
    void run() override
    {
      std::cerr << "[log] foo_runner::run: I'm foo and I'm running..." << std::endl;
      return;
    }

    // We declare the automatic registration mechanism of this class in the system register of its base class
    BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(examples::i_runner,
                                                           examples::foo_runner)
                                                           
  };

  // We implement the automatic registration of this class in the system register of its base class
  BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(examples::i_runner,
                                                              examples::foo_runner,
                                                              "examples::foo_runner")

} // end of namespace examples
 
namespace more_examples {

  /// Another concrete class (possibly from a client library) also inherited from the i_runner interface
  class bar_runner
    : public examples::i_runner
  {
  public:
     
    bar_runner()
    {
      std::cerr << "[log] bar_runner::ctor: Hello, bar!" << std::endl;
      return;
    }
    
    virtual ~bar_runner()
    {
      std::cerr << "[log] bar_runner::dtor: Bye, bar!" << std::endl;
      return;
    }
    
    void run() override
    {
      std::cerr << "[log] bar_runner::run: I'm bar and I'm running..." << std::endl;
      return;
    }

    // We declare the automatic registration mechanism of this class in the system register of its base class
    BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(examples::i_runner,
                                                           more_examples::bar_runner)
                                                           
  };

  // We implement the automatic registration of this class in the system register of its base class
  BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(examples::i_runner,
                                                              more_examples::bar_runner,
                                                              "more_examples::bar_runner")

  /// A concrete class inherited from the foo_runner concrete class above
  class baz_runner
    : public examples::foo_runner
  {
  public:
     
    baz_runner()
    {
      std::cerr << "[log] baz_runner::ctor: Hello, baz!" << std::endl;
      return;
    }
    
    virtual ~baz_runner()
    {
      std::cerr << "[log] baz_runner::dtor: Bye, baz!" << std::endl;
      return;
    }
    
    void run() override
    {
      std::cerr << "[log] baz_runner::run: I'm baz and I'm running..." << std::endl;
      return;
    }

    // We declare the automatic registration mechanism of this class in the system register of its super base class
    BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(examples::i_runner,
                                                           more_examples::baz_runner)
                                                           
  };

  // We implement the automatic registration of this class in the system register of its super base class
  BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(examples::i_runner,
                                                              more_examples::baz_runner,
                                                              "more_examples::baz_runner")


  /// Another concrete class (possibly from a client library) also inherited from the i_runner interface
  class special_runner
    : public examples::i_runner
  {
  public:
     
    special_runner()
    {
      std::cerr << "[log] special_runner::ctor: Hello, special!" << std::endl;
      return;
    }
    
    virtual ~special_runner()
    {
      std::cerr << "[log] special_runner::dtor: Bye, special!" << std::endl;
      return;
    }
    
    void run() override
    {
      std::cerr << "[log] special_runner::run: I'm special and I'm running..." << std::endl;
      return;
    }
                                                         
  };
  
} // end of namespace more_examples
 
int main(void)
{
  int error_code = EXIT_SUCCESS;
  try {

    {
      std::clog << std::endl << "================================================================" << std::endl;
      std::clog << "[log] Using the system/global factory register..." << std::endl;
      BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).print(std::clog,
                                                                        "[log] ",
                                                                        "System factory for classes inherited from example::i_runner: ");
      std::clog << std::endl;
    
      std::unique_ptr<examples::i_runner> runner1(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).get("examples::foo_runner")());
      std::unique_ptr<examples::i_runner> runner2(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).get("examples::foo_runner")());
      std::unique_ptr<examples::i_runner> runner3(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).get("more_examples::bar_runner")());
      std::unique_ptr<examples::i_runner> runner4(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).get("more_examples::baz_runner")());
      std::unique_ptr<examples::i_runner> runner5(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner).get("more_examples::bar_runner")());
      std::clog << std::endl;
    
      runner1->run();
      runner2->run();
      runner3->run();
      runner4->run();
      runner5->run();
      std::clog << std::endl;
    }

    {
      std::clog << std::endl << "================================================================" << std::endl;
      std::clog << "[log] Using a specific factory register, limited to only a few classes..." << std::endl;
      ::bxfactories::factory_register<examples::i_runner> myReg("Limited factory: ");
      myReg.import_some(BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(examples::i_runner),
                        std::set<std::string>({
                            "more_examples::bar_runner",
                              "more_examples::baz_runner"}));
      // Manual registration of the "special_runner" class:
      myReg.register_factory<more_examples::special_runner>("special", "A special runner");
      myReg.print(std::clog, "[log] ", "My own limited register: ");
      std::clog << std::endl;

      std::list<std::string> runner_type_ids({{"examples::foo_runner",
              "bad::no_runner",
              "more_examples::bar_runner",
              "special",
              "more_examples::baz_runner",
              "more_examples::dummy_runner"}});
      for (auto runner_type_id : runner_type_ids) {
        if (myReg.has(runner_type_id)) {
          std::clog << "[log] Runner with registration ID '" << runner_type_id << "' is to be instantiated." << std::endl;
          std::unique_ptr<examples::i_runner> runner(myReg.get(runner_type_id)());
          runner->run();
        } else {
          std::cerr << "[warning] Runner with registration ID '" << runner_type_id << "' does not exist in this register." << std::endl;
        }
      }
      std::clog << std::endl;

      // Manual unregistration of the "special_runner" class:
      myReg.unregister_factory("special");
   }
    
  } catch (std::exception & error) {
    std::cerr << "[error] " << error.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "[error] " << "Unexpected error!" << std::endl;
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
  
