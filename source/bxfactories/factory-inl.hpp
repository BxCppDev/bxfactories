/// \file bxfactories/factory-inl.hpp
/* Author(s)     : Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2020-03-18
 * Last modified : 2020-03-18
 *
 */

#ifndef BXFACTORIES_FACTORY_INL_HPP
#define BXFACTORIES_FACTORY_INL_HPP

// Implementation section for the factory_register class
namespace bxfactories {

  // template <typename BaseType>
  // factory_register<BaseType>::factory_register()
  //   : _label_()
  // {
  //   return;
  // }

  template <typename BaseType>
  factory_register<BaseType>::factory_register(const std::string & label_,
                                               const unsigned int flags_)
    : _label_(label_)
  {
    if (flags_ & init_trace) _trace_ = true;
    return;
  }

  template <typename BaseType>
  factory_register<BaseType>::~factory_register()
  {
    this->clear();
    return;
  }

  template <typename BaseType>
  const std::string & factory_register<BaseType>::get_label() const
  {
    return _label_;
  }

  template <typename BaseType>
  void factory_register<BaseType>::set_label(const std::string & label_)
  {
    _label_ = label_;
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::list_of_factory_ids(std::set<std::string> & ids_, bool clear_) const
  {
    if (clear_) ids_.clear(); // make sure the set is empty before to feed it
    for (typename factory_map_type::const_iterator i = _registered_.begin();
         i != _registered_.end();
         ++i) {
      ids_.insert(i->first);
    }
    return;
  }

  template <typename BaseType>
  bool factory_register<BaseType>::has(const std::string & id_) const
  {
    return _registered_.find(id_) != _registered_.end();
  }

  template <typename BaseType>
  void factory_register<BaseType>::clear()
  {
    for (typename factory_map_type::iterator i = _registered_.begin();
         i != _registered_.end();
         ++i) {
      if (i->second.fact != nullptr) {
        if (_trace_) {
          std::cerr << "[trace] bxfactory::factory_register<>::clear(): " << "Destroying registered allocator/functor '" << i->first << "'" << std::endl;
        }
      }
    }
    _registered_.clear();
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::reset()
  {
    this->clear();
    _label_.clear();
    _trace_ = false;
    return;
  }

  template <typename BaseType>
  typename factory_register<BaseType>::factory_type &
  factory_register<BaseType>::grab(const std::string & id_)
  {
    typename factory_map_type::iterator found = _registered_.find(id_);
    if (found == _registered_.end()) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::grab(...): " << "Class ID '" << id_ << "' is not registered !";
      throw std::logic_error(error_message.str());
    }
    return found->second.fact;
  }

  template <typename BaseType>
  const typename factory_register<BaseType>::factory_type &
  factory_register<BaseType>::get(const std::string & id_) const
  {
    typename factory_map_type::const_iterator found = _registered_.find(id_);
    if (found == _registered_.end()) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::get(...): " << "Class ID '" << id_ << "' is not registered !";
      throw std::logic_error(error_message.str());
    }
    return found->second.fact;
  }

  template <typename BaseType>
  const typename factory_register<BaseType>::factory_record_type &
  factory_register<BaseType>::get_record(const std::string & id_) const
  {
    typename factory_map_type::const_iterator found = _registered_.find(id_);
    if (found == _registered_.end()) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::get_record(...): " << "Class ID '" << id_ << "' is not registered !";
      throw std::logic_error(error_message.str());
    }
    return found->second;
  }
  
  template <typename BaseType>
  bool factory_register<BaseType>::fetch_type_id(const std::type_info & tinfo_, std::string & id_) const
  {
    id_.clear();
    for (typename factory_map_type::const_iterator i = _registered_.begin();
         i != _registered_.end();
         ++i) {
      if (&tinfo_ == i->second.tinfo) {
        id_ = i->first;
        return true;
      }
    }
    return false;
  }                    
 
  template <typename BaseType>
  template<class DerivedType>
  bool factory_register<BaseType>::fetch_type_id(std::string & id_) const
  {
    id_.clear();
    if (!std::is_base_of<BaseType, DerivedType>::value) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::fetch_type_id(...): " << "Class ID '" << id_ << "' is not registered !";
      throw std::logic_error(error_message.str());
    }
    for (typename factory_map_type::const_iterator i = _registered_.begin();
         i != _registered_.end();
         ++i) {
      if (&typeid(DerivedType) == i->second.tinfo) {
        id_ = i->first;
        return true;
      }
    }
    return false;
  }

  template <typename BaseType>
  template <typename DerivedType>
  void factory_register<BaseType>::register_factory(const std::string & id_,
                                                const std::string & description_,
                                                const std::string & category_)
  {
    register_factory(id_,
                     boost::factory<DerivedType*>(),
                     typeid(DerivedType),
                     description_,
                     category_);
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::register_factory(const std::string & id_,
                                                    const factory_type & factory_,
                                                    const std::type_info & tinfo_,
                                                    const std::string & description_,
                                                    const std::string & category_)
  {
    if (_trace_) std::cerr << "[trace] bxfactory::factory_register<>::register_factory(...): " << "Registration of class with ID '" << id_ << "'" << std::endl;
    typename factory_map_type::const_iterator found = _registered_.find(id_);
    if (found != _registered_.end()) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::register_factory(...): " << "Class ID '" << id_ << "' is already registered !";
      throw std::logic_error(error_message.str());
    }
    factory_record_type record;
    record.type_id = id_;
    record.description = description_;
    record.category = category_;
    record.fact = factory_;
    record.tinfo = &tinfo_;
    _registered_[id_] = record;
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::unregister_factory(const std::string & id_)
  {
    if (_trace_) std::cerr << "[trace] bxfactory::factory_register<>::unregistration(...): " << "Unregistration of class with ID '" << id_ << "'" << std::endl;
    typename factory_map_type::const_iterator found = _registered_.find(id_);
    if (found == _registered_.end()) {
      std::ostringstream error_message;
      error_message << "bxfactory::factory_register<>::unregister_factory(...): " << "Class ID '" << id_ << "' is not registered !";
      throw std::logic_error(error_message.str());
    }
    _registered_.erase(id_);
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::import(const factory_register & other_)
  {
    if (_trace_) std::cerr << "[trace] bxfactory::factory_register<>::import(...): " << "Importing registered factories from register '" << other_.get_label() << "'..." << std::endl;
    for (typename factory_map_type::const_iterator i = other_._registered_.begin();
         i != other_._registered_.end();
         ++i) {
      const factory_record_type & the_out_factory_record = i->second;
      this->registration(i->first,
                         the_out_factory_record.fact,
                         *the_out_factory_record.tinfo,
                         the_out_factory_record.description, 
                         the_out_factory_record.category);
    }
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::import_some(const factory_register & other_,
                                               const std::set<std::string> & imported_factories_)
  {
    if (this == &other_) return; // Should we throw ?
    if (_trace_) std::cerr << "[trace] bxfactory::factory_register<>::import_some(...): " << "Importing some registered factories from register '" << other_.get_label() << "'..." << std::endl;
    for (typename factory_map_type::const_iterator i = other_._registered_.begin();
         i != other_._registered_.end();
         ++i) {
      if (std::find(imported_factories_.begin(),
                    imported_factories_.end(),
                    i->first) != imported_factories_.end()) {
        if (_trace_) std::cerr << "[trace] bxfactory::factory_register<>::import_some(...): " << "Importing registered factory '" << i->first << "' from register '" << other_.get_label() << "'..." << std::endl;
        const factory_record_type & the_out_factory_record = i->second;
        this->register_factory(i->first,
                               the_out_factory_record.fact,
                               *the_out_factory_record.tinfo,
                               the_out_factory_record.description,
                               the_out_factory_record.category);
      }
    }
    return;
  }

  template <typename BaseType>
  void factory_register<BaseType>::print(std::ostream & out_,
                                         const std::string & indent_,
                                         const std::string & title_) const
  {
    static const std::string item_tag = "|-- ";
    static const std::string last_item_tag = "`-- ";
    static const std::string last_item_skip_tag = "    ";
    if (!title_.empty()) {
      out_ << indent_ << title_ << std::endl;
    }

    out_ << indent_ << item_tag
        << "Label   : '"
        << _label_ << "'" << std::endl;

    out_ << indent_ << last_item_tag
        << "Registered factories : " << _registered_.size() << std::endl;

    for (typename factory_map_type::const_iterator i = _registered_.begin();
         i != _registered_.end();
         ++i) {
      out_ << indent_;
      out_ << last_item_skip_tag;
      typename factory_map_type::const_iterator j = i;
      j++;
      if (j == _registered_.end()) {
        out_ << last_item_tag;
      } else {
        out_ << item_tag;
      }
      out_ << "ID: \"" << i->first << "\" @ " << &i->second.fact;
      if (!i->second.description.empty()) {
        out_ << ": " << i->second.description;
      }
      if (!i->second.category.empty()) {
        out_ << " (" << i->second.category << ')';
      }
      out_ << std::endl;
    }
    return;
  }

} // namespace bxfactories

#endif // BXFACTORIES_FACTORY_INL_HPP
