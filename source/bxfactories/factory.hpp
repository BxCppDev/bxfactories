/// \file bxfactories/factory.hpp
/* Author(s)     : Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2020-03-18
 * Last modified : 2020-03-18
 *
 */

#ifndef BXFACTORIES_FACTORY_HPP
#define BXFACTORIES_FACTORY_HPP

// Standard Library:
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

// Third Party:
// - Boost:
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>

namespace bxfactories {
  
  /*! \brief The base class for all specialized template factory registration classes
   */
  class base_factory_register
  {
  public:
    
    enum flag_type {
      init_trace = 0x1
    };

    /// Default constructor
    base_factory_register() = default;

    /// Destructor
    virtual ~base_factory_register() = default;
    
  };


  /// \brief Template factory registration class
  template <class BaseType>
  class factory_register
    : public base_factory_register
  {
  public:

    typedef BaseType                       base_type;
    typedef boost::function<base_type*() > factory_type;

    /// \brief Record for a factory
    struct factory_record_type {
      std::string  type_id;
      factory_type fact;
      const std::type_info * tinfo = nullptr;
      std::string  description;
      std::string  category;
    };
    
    /// \brief Dictionary of object factories
    typedef std::map<std::string, factory_record_type> factory_map_type;

    /// Constructor
    factory_register() = default;

    /// Constructor
    factory_register(const std::string & label_, const unsigned int flags_ = 0x0);

    /// Destructor
    virtual ~factory_register();

    //! Get the label associated to the factory
    const std::string & get_label() const;

    //! Set the label associated to the factory
    void set_label(const std::string & label_);

    /// Copy factory IDs into supplied container
    void list_of_factory_ids(std::set<std::string> & ids_, bool clear_ = false) const;

    /// Return true if a factory with given ID is registered
    bool has(const std::string & id_) const;

    /// Return true if a factory with given ID is registered with given group
    bool is_group(const std::string & id_) const;

    /// Clear all registered factories
    void clear();

    /// Reset the factory register
    void reset();

    /// Return a mutable reference to a factory given its registration ID
    factory_type & grab(const std::string & id_);

    /// Return a const reference to a factory given its registration ID
    const factory_type & get(const std::string & id_) const;

    /// Return a const reference to a factory record given its registration ID
    const factory_record_type & get_record(const std::string & id_) const;

    /// Register the supplied factory under the given ID
    void register_factory(const std::string & id_,
                          const factory_type & factory_,
                          const std::type_info & tinfo_,
                          const std::string & description_ = "",
                          const std::string & category_ = "");
    
    /// Register the supplied factory under the given ID
    template<class DerivedType>
    void register_factory(const std::string & id_,
                          const std::string & description_ = "",
                          const std::string & category_ = "");

    /// Fetch the registration type ID associated to a given class
    template<class DerivedType>
    bool fetch_type_id(std::string & id_) const;

    /// Fetch the registration type ID associated to a given type_info
    bool fetch_type_id(const std::type_info & tinfo_, std::string & id_) const;
                      
    /// Remove of the factory stored under supplied registration type ID
    void unregister_factory(const std::string & id_);

    /// Import all registered factories from another factory register
    void import(const factory_register & factory_register_);

    /// Import only registered factories addressed by their registration names
    /// from another factory register
    void import_some(const factory_register & factory_register_,
                     const std::set<std::string> & imported_factories_);

    /// Smart print for debugging/logging purpose
    void print(std::ostream & out_,
               const std::string & indent_ = "",
               const std::string & title_ = "") const;

  private:
    
    bool             _trace_ = false; ///< Trace log flag
    std::string      _label_;         ///< Label of the factory
    factory_map_type _registered_;    ///< Dictionary of registered factories

  };

} // end of namespace bxfactories

// Template definitions:
#include <bxfactories/factory-inl.hpp>

// Boost:
#include <type_traits>

namespace bxfactories {

  /// \brief Utility template class to enable auto-(un)registration of a derived class in a system factory register of a base class
  template <class BaseType, class DerivedType>
  class _system_factory_registrator
  {
  public:

    /// Constructor
    _system_factory_registrator(const std::string & type_id_)
      : _type_info_(typeid(DerivedType))
      , _type_id_( type_id_)
    {
      this->_trigger_factory_registration_();
      return;
    }

    /// Destructor
    ~_system_factory_registrator()
    {
      this->_trigger_factory_unregistration_();
      return;
    }

    /// Return registered type id
    const std::string & get_type_id()const
    {
      return _type_id_;
    }

  private:

    /// Factory registration
    void _trigger_factory_registration_()
    {
      if (!std::is_base_of<BaseType, DerivedType>::value) {
        std::ostringstream error_message;
        error_message << "Class ID '" << _type_id_ << "' cannot be registered in register '"
                      << BaseType::grab_system_factory_register().get_label() << "' !";
        throw std::logic_error(error_message.str());
      }
      BaseType::grab_system_factory_register().register_factory(_type_id_,
                                                                boost::factory<DerivedType*>(),
                                                                typeid(DerivedType));
      return;
    }

    /// Factory unregistration
    void _trigger_factory_unregistration_()
    {
      if (BaseType::grab_system_factory_register().has(_type_id_)) {
        BaseType::grab_system_factory_register().unregister_factory(_type_id_);
      }
      return;
    }

  private:

    const std::type_info & _type_info_; //!< System type info associated to the class of which the factory is auto-registered
    std::string _type_id_; //!< The registration type unique identifier of the auto-registered class
    
  };

} // end of namespace bxfactories

#endif // BXFACTORIES_FACTORY_HPP
