// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef DEBUG_ODB_HXX
#define DEBUG_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20300UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "debug.hxx"

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
  // debug
  //
  template <>
  struct class_traits< ::debug >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::debug >
  {
    public:
    typedef ::debug object_type;
    typedef ::debug* pointer_type;
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
  // debug
  //
  template <typename A>
  struct query_columns< ::debug, id_mysql, A >
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

    // compile_id
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    compile_id_type_;

    static const compile_id_type_ compile_id;

    // command
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        ::std::string,
        mysql::id_string >::query_type,
      mysql::id_string >
    command_type_;

    static const command_type_ command;

    // output
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        ::std::string,
        mysql::id_string >::query_type,
      mysql::id_string >
    output_type_;

    static const output_type_ output;

    // program_output
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        ::std::string,
        mysql::id_string >::query_type,
      mysql::id_string >
    program_output_type_;

    static const program_output_type_ program_output;

    // result
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    result_type_;

    static const result_type_ result;

    // current_line
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        unsigned int,
        mysql::id_ulong >::query_type,
      mysql::id_ulong >
    current_line_type_;

    static const current_line_type_ current_line;
  };

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::id_type_
  query_columns< ::debug, id_mysql, A >::
  id (A::table_name, "`id`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::compile_id_type_
  query_columns< ::debug, id_mysql, A >::
  compile_id (A::table_name, "`compile_id`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::command_type_
  query_columns< ::debug, id_mysql, A >::
  command (A::table_name, "`command`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::output_type_
  query_columns< ::debug, id_mysql, A >::
  output (A::table_name, "`output`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::program_output_type_
  query_columns< ::debug, id_mysql, A >::
  program_output (A::table_name, "`program_output`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::result_type_
  query_columns< ::debug, id_mysql, A >::
  result (A::table_name, "`result`", 0);

  template <typename A>
  const typename query_columns< ::debug, id_mysql, A >::current_line_type_
  query_columns< ::debug, id_mysql, A >::
  current_line (A::table_name, "`current_line`", 0);

  template <typename A>
  struct pointer_query_columns< ::debug, id_mysql, A >:
    query_columns< ::debug, id_mysql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::debug, id_mysql >:
    public access::object_traits< ::debug >
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

      // compile_id_
      //
      unsigned int compile_id_value;
      my_bool compile_id_null;

      // command_
      //
      details::buffer command_value;
      unsigned long command_size;
      my_bool command_null;

      // output_
      //
      details::buffer output_value;
      unsigned long output_size;
      my_bool output_null;

      // program_output_
      //
      details::buffer program_output_value;
      unsigned long program_output_size;
      my_bool program_output_null;

      // result_
      //
      unsigned int result_value;
      my_bool result_null;

      // current_line_
      //
      unsigned int current_line_value;
      my_bool current_line_null;

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

    static const std::size_t column_count = 7UL;
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
  class access::object_traits_impl< ::debug, id_common >:
    public access::object_traits_impl< ::debug, id_mysql >
  {
  };

  // debug
  //
}

#include "debug-odb.ixx"

#include <odb/post.hxx>

#endif // DEBUG_ODB_HXX
