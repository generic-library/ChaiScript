// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2011, Jonathan Turner (jonathan@emptycrate.com)
// and Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_BOOTSTRAP_HPP_
#define CHAISCRIPT_BOOTSTRAP_HPP_

#include "dispatchkit.hpp"
#include "dynamic_object.hpp"
#include "register_function.hpp"
#include "operators.hpp"
#include "boxed_numeric.hpp"
#include <boost/function_types/result_type.hpp>

namespace chaiscript 
{
  /// \brief Classes and functions useful for bootstrapping of ChaiScript and adding of new types
  namespace bootstrap
  {
    namespace detail
    {

      /// \brief Assigns a POD value from a Boxed_Numeric. Helps support operators between
      ///        disparate POD types.
      /// \param[in,out] p1 object to assign to
      /// \param[in] v Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_pod(P1 &p1, const Boxed_Numeric &v)
      {
        if (v.isfloat)
        {
          return (p1 = P1(v.d));
        } else {
          return (p1 = P1(v.i));
        }
      }

      /// \brief Constructs a new POD value object from a Boxed_Numeric
      /// \param[in] v Boxed_Numeric to copy into the new object
      /// \returns The newly created object.
      template<typename P1>
      P1 construct_pod(Boxed_Numeric v)
      {
        if (v.isfloat)
        {
          return P1(v.d);
        } else {
          return P1(v.i);
        }    
      }

      /// \brief Performs a bitwise and assignment (&=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to bitwise and assign to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_bitwise_and_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 &= P1(r.i);
        }

        throw exception::bad_boxed_cast("&= only valid for integer types");
      }

      /// \brief Performs a xor assignment (^=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to xor assign to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_xor_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 ^= P1(r.i);
        }

        throw exception::bad_boxed_cast("^= only valid for integer types");
      }

      /// \brief Performs a bitwise or assignment (|=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to bitwise or assign to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_bitwise_or_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 |= P1(r.i);
        }

        throw exception::bad_boxed_cast("&= only valid for integer types");
      }

      /// \brief Performs an assign difference (-=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to difference assign to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_difference_pod(P1 &p1, Boxed_Numeric r)
      {
        if (r.isfloat)
        {
          return p1 -= P1(r.d);
        } else {
          return p1 -= P1(r.i);
        }
      }

      /// \brief Performs an assign shift left (<<=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to assign shift left to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_left_shift_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 <<= P1(r.i);
        }

        throw exception::bad_boxed_cast("<<= only valid for integer types");
      }


      /// \brief Performs an assign product (*=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to assign product to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_product_pod(P1 &p1, Boxed_Numeric r)
      {
        if (r.isfloat)
        {
          return p1 *= P1(r.d);
        } else {
          return p1 *= P1(r.i);
        }
      }

      /// \brief Performs an assign quotient (/=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to assign quotient to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_quotient_pod(P1 &p1, Boxed_Numeric r)
      {
        if (r.isfloat)
        {
          return p1 /= P1(r.d);
        } else {
          return p1 /= P1(r.i);
        }
      }

      /// \brief Performs an assign remainder (%=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to assign remainder to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_remainder_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 %= P1(r.i);
        }

        throw exception::bad_boxed_cast("%= only valid for integer types");
      }


      /// \brief Performs an assign shift right (>>=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to assign shift right to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_right_shift_pod(P1 &p1, Boxed_Numeric r)
      {
        if (!r.isfloat)
        {
          return p1 >>= P1(r.i);
        }

        throw exception::bad_boxed_cast(">>= only valid for integer types");
      }

      /// \brief Performs an assign sum (+=) on the given object with the given Boxed_Numeric
      /// \param[in,out] p1 object to sum assign to
      /// \param[in] r Boxed_Numeric to assign from
      /// \returns Reference to p1, to support normal C assignment semantics
      template<typename P1>
      P1 &assign_sum_pod(P1 &p1, Boxed_Numeric r)
      {
        if (r.isfloat)
        {
          return p1 += P1(r.d);
        } else {
          return p1 += P1(r.i);
        }
      }

    }

    /// \brief Add all comparison operators for the templated type. Used during bootstrap, also available to users.
    /// \tparam T Type to create comparison operators for
    /// \param[in,out] m module to add comparison operators to
    /// \returns the passed in ModulePtr or the newly constructed one if the default params are used.
    template<typename T>
    ModulePtr opers_comparison(ModulePtr m = ModulePtr(new Module()))
    {
      operators::equal<T>(m);
      operators::greater_than<T>(m);
      operators::greater_than_equal<T>(m);
      operators::less_than<T>(m);
      operators::less_than_equal<T>(m);
      operators::not_equal<T>(m);
      return m;
    }


    /// \brief Add all arithmetic operators appropriate for integers for the templated type. 
    ///        Used during bootstrap, also available to users.
    /// \tparam T Type to create arithmetic operators for
    /// \param[in,out] m module to add arithmetic operators to
    /// \returns the passed in ModulePtr or the newly constructed one if the default params are used.
    template<typename T>
    ModulePtr opers_integer_arithmetic(ModulePtr m = ModulePtr(new Module()))
    {
      operators::assign_bitwise_and<T>(m);
      operators::assign_xor<T>(m);
      operators::assign_bitwise_or<T>(m);
      operators::assign_difference<T>(m);
      operators::assign_left_shift<T>(m);
      operators::assign_product<T>(m);
      operators::assign_quotient<T>(m);
      operators::assign_remainder<T>(m);
      operators::assign_right_shift<T>(m);
      operators::assign_sum<T>(m);

      operators::prefix_decrement<T>(m);
      operators::prefix_increment<T>(m);
      operators::addition<T>(m);
      operators::unary_plus<T>(m);
      operators::subtraction<T>(m);
      operators::unary_minus<T>(m);
      operators::bitwise_and<T>(m);
      operators::bitwise_compliment<T>(m);
      operators::bitwise_xor<T>(m);
      operators::bitwise_or<T>(m);
      operators::division<T>(m);
      operators::left_shift<T>(m);
      operators::multiplication<T>(m);
      operators::remainder<T>(m);
      operators::right_shift<T>(m);
      return m;
    }

    /// \brief Add all arithmetic operators appropriate for floating point numbers for the templated type. 
    ///        Used during bootstrap, also available to users.
    /// \tparam T Type to create arithmetic operators for
    /// \param[in,out] m module to add arithmetic operators to
    /// \returns the passed in ModulePtr or the newly constructed one if the default params are used.
    template<typename T>
    ModulePtr opers_float_arithmetic(ModulePtr m = ModulePtr(new Module()))
    {
      operators::assign_difference<T>(m);
      operators::assign_product<T>(m);
      operators::assign_quotient<T>(m);
      operators::assign_sum<T>(m);

      operators::addition<T>(m);
      operators::unary_plus<T>(m);
      operators::subtraction<T>(m);
      operators::unary_minus<T>(m);
      operators::division<T>(m);
      operators::multiplication<T>(m);
      return m;
    }

    /// \brief Adds a copy constructor for the given type to the given Model
    /// \param[in] type The name of the type. The copy constructor will be named "type".
    /// \param[in,out] m The Module to add the copy constructor to
    /// \tparam T The type to add a copy constructor for
    /// \returns The passed in ModulePtr, or the newly constructed one if the default param is used
    template<typename T>
    ModulePtr copy_constructor(const std::string &type, ModulePtr m = ModulePtr(new Module()))
    {
      m->add(constructor<T (const T &)>(), type);
      return m;
    }

    /// \brief Adds default and copy constructors for the given type
    /// \param[in] type The name of the type to add the constructors for.
    /// \param[in,out] m The Module to add the basic constructors to
    /// \tparam T Type to generate basic constructors for
    /// \returns The passed in ModulePtr, or the newly constructed one if the default param is used
    /// \sa copy_constructor
    /// \sa constructor
    template<typename T>
    ModulePtr basic_constructors(const std::string &type, ModulePtr m = ModulePtr(new Module()))
    {
      m->add(constructor<T ()>(), type);
      copy_constructor<T>(type, m);
      return m;
    }

    /// \brief Adds a constructor for a POD type 
    /// \tparam T The type to add the constructor for
    /// \param[in] type The name of the type
    /// \param[in,out] m The Module to add the constructor to
    template<typename T>
    ModulePtr construct_pod(const std::string &type, ModulePtr m = ModulePtr(new Module()))
    {
      m->add(fun(&detail::construct_pod<T>), type);
      return m;
    }


    /**
    * to_string function for internal use. Uses ostream operator<<
    */
    template<typename Input>
    std::string to_string(Input i)
    {
      return boost::lexical_cast<std::string>(i);
    }


    /**
    * Internal function for converting from a string to a value
    * uses ostream operator >> to perform the conversion
    */
    template<typename Input>
    Input parse_string(const std::string &i)
    {
      return boost::lexical_cast<Input>(i);
    }


    /**
     * Add assignment operator for T = POD.
     */
    template<typename T>
      ModulePtr oper_assign_pod(ModulePtr m = ModulePtr(new Module()))
      {
        m->add(fun(&detail::assign_pod<T>), "=");
        return m;
      }

    /**
    * Add all common functions for a POD type. All operators, and
    * common conversions
    */
    template<typename T>
    ModulePtr bootstrap_pod_type(const std::string &name, ModulePtr m = ModulePtr(new Module()))
    {
      m->add(user_type<T>(), name);
      basic_constructors<T>(name, m);
      operators::assign<T>(m);
      oper_assign_pod<T>(m);
      construct_pod<T>(name, m);

      m->add(fun(&detail::assign_sum_pod<T>), "+=");
      m->add(fun(&detail::assign_difference_pod<T>), "-=");
      m->add(fun(&detail::assign_product_pod<T>), "*=");
      m->add(fun(&detail::assign_quotient_pod<T>), "/=");

      m->add(fun(&to_string<T>), "to_string");
      m->add(fun(&parse_string<T>), "to_" + name);
      return m;
    }

    /**
    * Add all common functions for a POD type. All operators, and
    * common conversions
    */
    template<typename T>
    ModulePtr bootstrap_integer_type(const std::string &name, ModulePtr m = ModulePtr(new Module()))
    {
      bootstrap_pod_type<T>(name, m);

      m->add(fun(&detail::assign_bitwise_and_pod<T>), "&=");
      m->add(fun(&detail::assign_xor_pod<T>), "^=");
      m->add(fun(&detail::assign_bitwise_or_pod<T>), "|=");
      m->add(fun(&detail::assign_left_shift_pod<T>), "<<=");
      m->add(fun(&detail::assign_remainder_pod<T>), "%=");
      m->add(fun(&detail::assign_right_shift_pod<T>), ">>=");

      opers_integer_arithmetic<T>(m);
      return m;
    }

    /**
    * Add all common functions for a POD type. All operators, and
    * common conversions
    */
    template<typename T>
    ModulePtr bootstrap_float_type(const std::string &name, ModulePtr m = ModulePtr(new Module()))
    {
      bootstrap_pod_type<T>(name, m);
      opers_float_arithmetic<T>(m);
      return m;
    }

    /**
    * "clone" function for a shared_ptr type. This is used in the case
    * where you do not want to make a deep copy of an object during cloning
    * but want to instead maintain the shared_ptr. It is needed internally
    * for handling of Proxy_Function object (that is,
    * function variables.
    */
    template<typename Type>
    boost::shared_ptr<Type> shared_ptr_clone(const boost::shared_ptr<Type> &p)
    {
      return p;
    }

    /**
     * Specific version of shared_ptr_clone just for Proxy_Functions
     */
    template<typename Type>
    boost::shared_ptr<typename boost::remove_const<Type>::type> 
        shared_ptr_unconst_clone(const boost::shared_ptr<typename boost::add_const<Type>::type> &p)
    {
      return boost::const_pointer_cast<typename boost::remove_const<Type>::type>(p);
    }



    /**
    * Assignment function for shared_ptr objects, does not perform a copy of the
    * object pointed to, instead maintains the shared_ptr concept.
    * Similar to shared_ptr_clone. Used for Proxy_Function.
    */
    template<typename Type>
    Boxed_Value ptr_assign(Boxed_Value lhs, const boost::shared_ptr<Type> &rhs)
    {
      if (lhs.is_undef() 
          || (!lhs.get_type_info().is_const() && lhs.get_type_info().bare_equal(chaiscript::detail::Get_Type_Info<Type>::get())))
      {
        lhs.assign(Boxed_Value(rhs));
        return lhs;
      } else {
        throw exception::bad_boxed_cast("type mismatch in pointer assignment");
      }
    }

    /**
    * Class consisting of only static functions. All default bootstrapping occurs
    * from this class.
    */
    class Bootstrap
    {
    private:
      /**
      * Function allowing for assignment of an unknown type to any other value
      */
      static Boxed_Value unknown_assign(Boxed_Value lhs, Boxed_Value rhs)
      {
        if (lhs.is_undef())
        {
          return (lhs.assign(rhs));
        } else {
          throw exception::bad_boxed_cast("boxed_value has a set type already");
        }
      }

      static void print(const std::string &s)
      {
        std::cout << s;
      }

      static void println(const std::string &s)
      {
        std::cout << s << std::endl;
      }

      /**
      * Add all arithmetic operators for PODs
      */
      static void opers_arithmetic_pod(ModulePtr m = ModulePtr(new Module()))
      {
        m->add(fun(&operators::addition<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "+");
        m->add(fun(&operators::subtraction<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "-");
        m->add(fun(&operators::bitwise_and<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "&");
        m->add(fun(&operators::bitwise_xor<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "^");
        m->add(fun(&operators::bitwise_or<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "|");
        m->add(fun(&operators::division<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "/");
        m->add(fun(&operators::left_shift<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "<<");
        m->add(fun(&operators::multiplication<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "*");
        m->add(fun(&operators::remainder<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), "%");
        m->add(fun(&operators::right_shift<Boxed_Value, Boxed_Numeric, Boxed_Numeric>), ">>");
      }

      /**
      * Create a bound function object. The first param is the function to bind
      * the remaining parameters are the args to bind into the
      * result
      */
      static Boxed_Value bind_function(const std::vector<Boxed_Value> &params)
      {
        if (params.size() < 2)
        {
          throw exception::arity_error(static_cast<int>(params.size()), 2);
        }

        Const_Proxy_Function f = boxed_cast<Const_Proxy_Function>(params[0]);

        return Boxed_Value(Const_Proxy_Function(new dispatch::Bound_Function(f,
          std::vector<Boxed_Value>(params.begin() + 1, params.end()))));
      }

      /**
      * Returns true if a call can be made that consists of the first parameter
      * (the function) with the remaining parameters as its arguments.
      */
      static Boxed_Value call_exists(const std::vector<Boxed_Value> &params)
      {
        if (params.size() < 1)
        {
          throw exception::arity_error(static_cast<int>(params.size()), 1);
        }

        Const_Proxy_Function f = boxed_cast<Const_Proxy_Function>(params[0]);

        return Boxed_Value(f->call_match(std::vector<Boxed_Value>(params.begin() + 1, params.end())));
      }

      static bool has_guard(const Const_Proxy_Function &t_pf)
      {
        boost::shared_ptr<const dispatch::Dynamic_Proxy_Function> pf = boost::dynamic_pointer_cast<const dispatch::Dynamic_Proxy_Function>(t_pf);
        if (pf)
        {
          return pf->get_guard();
        } else {
          return false;
        }
      }

      static Const_Proxy_Function get_guard(const Const_Proxy_Function &t_pf)
      {
        boost::shared_ptr<const dispatch::Dynamic_Proxy_Function> pf = boost::dynamic_pointer_cast<const dispatch::Dynamic_Proxy_Function>(t_pf);
        if (pf)
        {
          if (pf->get_guard())
          {
            return pf->get_guard();
          } else {
            throw std::runtime_error("Function does not have a guard");
          }
        } else {
          throw std::runtime_error("Function does not have a guard");
        }
      }

      static void throw_exception(const Boxed_Value &bv) {
        throw bv;
      }
      
      static boost::shared_ptr<chaiscript::detail::Dispatch_Engine> bootstrap2(boost::shared_ptr<chaiscript::detail::Dispatch_Engine> e = boost::shared_ptr<chaiscript::detail::Dispatch_Engine> (new chaiscript::detail::Dispatch_Engine()))
      {
        e->add(user_type<void>(), "void");
        return e;
      }

      static std::string what(const std::exception &e)
      {
        return e.what();
      }

      /**
       * Boolean specialization of internal to_string function 
       */
      static std::string bool_to_string(bool b)
      {
        if (b)
        {
          return "true";
        } else {
          return "false";
        }
      }

      template<typename FunctionType>
        static std::vector<Boxed_Value> do_return_boxed_value_vector(FunctionType f,
            const dispatch::Proxy_Function_Base *b)
        {
          typedef typename boost::function_types::result_type<FunctionType>::type Vector;
          Vector v = (b->*f)();
 
          std::vector<Boxed_Value> vbv;
          for (typename Vector::const_iterator itr = v.begin();
               itr != v.end();
               ++itr)
          {
            vbv.push_back(const_var(*itr));
          }

          return vbv;
        }

      template<typename Function>
      static boost::function<std::vector<Boxed_Value> (const dispatch::Proxy_Function_Base*)> return_boxed_value_vector(const Function &f)
      {
        return boost::bind(&do_return_boxed_value_vector<Function>, f, _1);
      }

    public:
      /// \brief perform all common bootstrap functions for std::string, void and POD types
      /// \param[in,out] m Module to add bootstrapped functions to
      /// \returns passed in ModulePtr, or newly created one if default argument is used
      static ModulePtr bootstrap(ModulePtr m = ModulePtr(new Module()))
      {
        m->add(user_type<void>(), "void");
        m->add(user_type<bool>(), "bool");
        m->add(user_type<Boxed_Value>(), "Object");
        m->add(user_type<Boxed_Numeric>(), "PODObject");
        m->add(user_type<Proxy_Function>(), "Function");
        m->add(user_type<std::exception>(), "exception");

        m->add(fun(&dispatch::Proxy_Function_Base::get_arity), "get_arity");
        m->add(fun(&dispatch::Proxy_Function_Base::annotation), "get_annotation");
        m->add(fun(&dispatch::Proxy_Function_Base::operator()), "call");
        m->add(fun(&dispatch::Proxy_Function_Base::operator==), "==");

        
        m->add(fun(return_boxed_value_vector(&dispatch::Proxy_Function_Base::get_param_types)), "get_param_types");
        m->add(fun(return_boxed_value_vector(&dispatch::Proxy_Function_Base::get_contained_functions)), "get_contained_functions");


        m->add(user_type<std::runtime_error>(), "runtime_error");
        m->add(chaiscript::base_class<std::exception, std::runtime_error>());
 

        m->add(constructor<std::runtime_error (const std::string &)>(), "runtime_error");
        m->add(fun(boost::function<std::string (const std::runtime_error &)>(&what)), "what");     

        m->add(user_type<dispatch::Dynamic_Object>(), "Dynamic_Object");
        m->add(constructor<dispatch::Dynamic_Object (const std::string &)>(), "Dynamic_Object");
        m->add(fun(&dispatch::Dynamic_Object::get_type_name), "get_type_name");
        m->add(fun(&dispatch::Dynamic_Object::get_attrs), "get_attrs");
        m->add(fun(&dispatch::Dynamic_Object::get_attr), "get_attr");

        m->eval("def Dynamic_Object::clone() { var new_o := Dynamic_Object(this.get_type_name()); for_each(this.get_attrs(), bind(fun(new_o, x) { new_o.get_attr(x.first) = x.second; }, new_o, _) ); return new_o; }");

        m->add(fun(&has_guard), "has_guard");
        m->add(fun(&get_guard), "get_guard");

        m->add(fun(&Boxed_Value::is_undef), "is_var_undef");
        m->add(fun(&Boxed_Value::is_null), "is_var_null");
        m->add(fun(&Boxed_Value::is_const), "is_var_const");
        m->add(fun(&Boxed_Value::is_ref), "is_var_reference");
        m->add(fun(&Boxed_Value::is_pointer), "is_var_pointer");
        m->add(fun(&Boxed_Value::is_type), "is_type");

        m->add(fun(&Boxed_Value::get_type_info), "get_type_info");
        m->add(user_type<Type_Info>(), "Type_Info");


        operators::equal<Type_Info>(m);

        m->add(fun(&Type_Info::is_const), "is_type_const");
        m->add(fun(&Type_Info::is_reference), "is_type_reference");
        m->add(fun(&Type_Info::is_void), "is_type_void");
        m->add(fun(&Type_Info::is_undef), "is_type_undef");
        m->add(fun(&Type_Info::is_pointer), "is_type_pointer");
        m->add(fun(&Type_Info::name), "cpp_name");
        m->add(fun(&Type_Info::bare_name), "cpp_bare_name");
        m->add(fun(&Type_Info::bare_equal), "bare_equal");
        typedef bool (Type_Info::*typeinfocompare)(const Type_Info &) const;
        m->add(fun(typeinfocompare(&Type_Info::operator==)), "==");
        m->add(fun(&Type_Info::bare_equal), "bare_equal");


        basic_constructors<bool>("bool", m);
        operators::assign<bool>(m);

        m->add(fun(&to_string<const std::string &>), "internal_to_string");
        m->add(fun(&Bootstrap::bool_to_string), "internal_to_string");
        m->add(fun(&unknown_assign), "=");
        m->add(fun(&throw_exception), "throw");
        m->add(fun(&what), "what");

        bootstrap_float_type<double>("double", m);
        bootstrap_integer_type<int>("int", m);
        bootstrap_integer_type<size_t>("size_t", m);
        bootstrap_integer_type<char>("char", m);
        bootstrap_integer_type<boost::int64_t>("int64_t", m);

        operators::logical_compliment<bool>(m);

        opers_comparison<Boxed_Numeric>(m);
        opers_arithmetic_pod(m);


        m->add(fun(&print), "print_string");
        m->add(fun(&println), "println_string");

        m->add(Proxy_Function(new dispatch::Dynamic_Proxy_Function(boost::bind(&bind_function, _1))), 
          "bind");

        m->add(fun(&shared_ptr_unconst_clone<dispatch::Proxy_Function_Base>), "clone");
        m->add(fun(&ptr_assign<boost::remove_const<dispatch::Proxy_Function_Base>::type>), "=");
        m->add(fun(&ptr_assign<boost::add_const<dispatch::Proxy_Function_Base>::type>), "=");

        m->add(Proxy_Function(new dispatch::Dynamic_Proxy_Function(boost::bind(&call_exists, _1))), 
          "call_exists");

        m->add(fun(&type_match), "type_match");

        return m;
      }
    };
  }
}

#endif

