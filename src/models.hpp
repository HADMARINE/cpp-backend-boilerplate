//
// Created by HADMARINE on 2020/10/02.
//

#ifndef CPP_BACKEND_BOILERPLATE_MODELS_HPP
#define CPP_BACKEND_BOILERPLATE_MODELS_HPP

#include "pch.hpp"

namespace models {
  class Model;
  
  
  enum class DATA_FLAGS {
    LOWER_CASE, UPPER_CASE,
    IS_REQUIRED, AUTO_INCREMENT
  };
  
  template <typename DATA_TYPE>
  class Data{
  public:
    Data* setLowerCase(bool b);
    Data* setUpperCase(bool b);
    Data* setIsRequired(bool b);
    Data* setValue(DATA_TYPE v);
    Data* setDefaultValue(DATA_TYPE v);
    Data* setReferenceModel(Model* r);
    Data* setDataFieldName(std::string n);
    Data* setAutoIncrement(bool b);
    Data* setGetter(std::function<DATA_TYPE(DATA_TYPE)> f);
    Data* setSetter(std::function<DATA_TYPE(DATA_TYPE)> f);
    Data* setMinLength(uint32_t len);
    Data* setMaxLength(uint32_t len);
    
    // UTILS
    Data* setFlag(std::initializer_list<DATA_FLAGS>);
  private:
    DATA_TYPE default_value = NULL;
    DATA_TYPE value = default_value;
    bool lowercase = false;
    bool uppercase = false;
    bool required = false;
    Model* ref = nullptr;
    std::string name = nullptr;
    uint32_t minLength = 0;
    uint32_t maxLength = 0;
    std::function<DATA_TYPE(DATA_TYPE)> getter;
    std::function<DATA_TYPE(DATA_TYPE)> setter;
  };
  
  
  class Model {
  public:
    template <typename T>
    Model* appendData(std::initializer_list<Data<T>>);
  private:
    template <typename T>
    std::initializer_list<Data<T>>;
  };
  

  
  
  

}

#endif//CPP_BACKEND_BOILERPLATE_MODELS_HPP
