/// \file bxfactories/factory_macros.hpp
/* Author(s)     : Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2020-03-18
 * Last modified : 2020-03-18
 *
 */
#ifndef BXFACTORIES_FACTORY_MACROS_HPP
#define BXFACTORIES_FACTORY_MACROS_HPP

// Standard Library:
#include <memory>

// Third Party:
#include <boost/preprocessor/stringize.hpp>

// This project:
#include <bxfactories/factory.hpp>

/// These macros provide some automated mechanisms to :
///  - setup a global factory register associated to a given base class;
///  - automate the registration of a factory object associated to any daughter class
///    in the global factory register associated to the mother base class.
///
/// Example:
/// \code
/// // In header:
/// class Base {
/// public:
///   Base() = default;
///   virtual ~Base() = default;
///   BXFACTORIES_FACTORY_SYSTEM_REGISTER_INTERFACE(Base)
/// };
///
/// // In implementation:
/// BXFACTORIES_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(Base)
///
/// // In header:
/// class Derived1 : public Base {
/// public:
///   Derived1() = default;
///   BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(Base, Derived1)
/// };
///
/// BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(Base, Derived1, "Derived1")
///
/// // In header:
/// class Derived2 : public Base {
/// public:
///   Derived2() = default;
///   BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(Base, Derived2)
/// };
///
/// // In implementation:
/// BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(Base, Derived2, "Derived2")
/// \endcode

/// Alias for a factory register associated to a base class
#define BXFACTORIES_FACTORY_INTERFACE(BaseType)                         \
  public:                                                               \
  typedef ::bxfactories::factory_register< BaseType > factory_register_type; \
  /**/

/// Declaration of a system (allocator/functor) factory register as a static member of a base class and static accessor methods
#define BXFACTORIES_FACTORY_SYSTEM_REGISTER_INTERFACE(BaseType)         \
  BXFACTORIES_FACTORY_INTERFACE(BaseType)                               \
  public:                                                               \
  typedef ::std::unique_ptr< factory_register_type > scoped_factory_register_type; \
  static factory_register_type& grab_system_factory_register();         \
  static const factory_register_type& get_system_factory_register();    \
  /**/

/// Instantiate the system (allocator/functor) factory register and its associated accessors
#define BXFACTORIES_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(BaseType, RegisterLabel) \
  BaseType::factory_register_type& BaseType::grab_system_factory_register() \
  {                                                                     \
    static scoped_factory_register_type _system_factory_register;       \
    if (!_system_factory_register) {                                    \
      _system_factory_register.reset(new BaseType::factory_register_type(RegisterLabel, 0)); \
    }                                                                   \
    return *_system_factory_register.get();                             \
  }                                                                     \
  const BaseType::factory_register_type& BaseType::get_system_factory_register() \
  {                                                                     \
    BaseType::factory_register_type& fr = BaseType::grab_system_factory_register(); \
    return const_cast< BaseType::factory_register_type& >(fr);          \
  }                                                                     \
  /**/

// Useful macros
#define BXFACTORIES_FACTORY_GRAB_SYSTEM_REGISTER(BaseType)      \
  BaseType::grab_system_factory_register()                      \
  /**/

#define BXFACTORIES_FACTORY_GET_SYSTEM_REGISTER(BaseType)       \
  BaseType::get_system_factory_register()                       \
  /**/

/// Interface macro of the automated registration for derived classes
#define BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(BaseType, DerivedType) \
  private:                                                              \
  static ::bxfactories::_system_factory_registrator< BaseType , DerivedType > _g_system_factory_auto_registration_; \
public:                                                                 \
 static const std::string & system_factory_auto_registration_id();      \
 /**/

/// Implementation macro of the automated registration for derived classes
#define BXFACTORIES_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(BaseType, DerivedType, DerivedTypeId) \
  ::bxfactories::_system_factory_registrator< BaseType , DerivedType > DerivedType::_g_system_factory_auto_registration_(DerivedTypeId); \
  const std::string & DerivedType::system_factory_auto_registration_id() \
  {                                                                     \
    static const std::string _id(DerivedTypeId);                        \
    return _id;                                                         \
  }                                                                     \
  /**/

#endif // BXFACTORIES_FACTORY_MACROS_HPP
