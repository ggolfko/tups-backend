// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef PROBLEM_ODB_HXX
#define PROBLEM_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20300UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "problem.hxx"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // problem
  //
  template <>
  struct class_traits< ::problem >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::problem >
  {
    public:
    typedef ::problem object_type;
    typedef ::problem* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef unsigned int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/mysql/version.hxx>
#include <odb/mysql/forward.hxx>
#include <odb/mysql/binding.hxx>
#include <odb/mysql/mysql-types.hxx>
#include <odb/mysql/query.hxx>

namespace odb
{
  // problem
  //
  template <typename A>
  struct query_columns< ::problem, id_mysql, A >
  {
    // id
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    id_type_;

    static const id_type_ id;

    // time_limit
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    time_limit_type_;

    static const time_limit_type_ time_limit;

    // memory_limit
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    memory_limit_type_;

    static const memory_limit_type_ memory_limit;

    // diff_option
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    diff_option_type_;

    static const diff_option_type_ diff_option;
  };

  template <typename A>
  const typename query_columns< ::problem, id_mysql, A >::id_type_
  query_columns< ::problem, id_mysql, A >::
  id (A::table_name, "`id`", 0);

  template <typename A>
  const typename query_columns< ::problem, id_mysql, A >::time_limit_type_
  query_columns< ::problem, id_mysql, A >::
  time_limit (A::table_name, "`time_limit`", 0);

  template <typename A>
  const typename query_columns< ::problem, id_mysql, A >::memory_limit_type_
  query_columns< ::problem, id_mysql, A >::
  memory_limit (A::table_name, "`memory_limit`", 0);

  template <typename A>
  const typename query_columns< ::problem, id_mysql, A >::diff_option_type_
  query_columns< ::problem, id_mysql, A >::
  diff_option (A::table_name, "`diff_option`", 0);

  template <typename A>
  struct pointer_query_columns< ::problem, id_mysql, A >:
    query_columns< ::problem, id_mysql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::problem, id_mysql >:
    public access::object_traits< ::problem >
  {
    public:
    struct id_image_type
    {
      unsigned int id_value;
      my_bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id_
      //
      unsigned int id_value;
      my_bool id_null;

      // time_limit_
      //
      unsigned int time_limit_value;
      my_bool time_limit_null;

      // memory_limit_
      //
      unsigned int memory_limit_value;
      my_bool memory_limit_null;

      // diff_option_
      //
      unsigned int diff_option_value;
      my_bool diff_option_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          my_bool*);

    static void
    bind (MYSQL_BIND*,
          image_type&,
          mysql::statement_kind);

    static void
    bind (MYSQL_BIND*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          mysql::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef mysql::object_statements<object_type> statements_type;

    typedef mysql::query_base query_base_type;

    static const std::size_t column_count = 4UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::problem, id_common >:
    public access::object_traits_impl< ::problem, id_mysql >
  {
  };

  // problem
  //
}

#include "problem-odb.ixx"

#include <odb/post.hxx>

#endif // PROBLEM_ODB_HXX