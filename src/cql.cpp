/*
  Copyright (c) 2014 DataStax

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <string>

#include "cql.h"
#include "cql_session.hpp"

extern "C" {

CqlSession* cql_session_new() {
  return new CqlSession();
}

CqlSession* cql_session_clone(CqlSession* session) {
  return new CqlSession(session);
}

void
cql_session_free(
   CqlSession* session) {
  delete session;
}

int
cql_session_setopt(
    CqlSession* session,
    CqlOption   option,
    const void* data,
    size_t      data_len) {
  return session->config_.option(option, data, data_len);
}

int
cql_session_getopt(
    CqlSession* session,
    CqlOption   option,
    void**      data,
    size_t*     data_len)
{
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_connect(
   CqlSession* session,
   CqlFuture** future) {
  *future = session->connect("");
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_connect_keyspace(
   CqlSession*  session,
    const char* keyspace,
    CqlFuture** future) {
  *future = session->connect(keyspace);
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_shutdown(
    CqlSession* session,
    CqlFuture** future) {
  *future = session->shutdown();
  return CQL_ERROR_NO_ERROR;
}

void
cql_future_free(
   CqlFuture* future) {
  delete future;
}

int
cql_future_ready(
   CqlFuture* future) {
  return static_cast<int>(future->ready());
}

void
cql_future_wait(
   CqlFuture* future) {
  future->wait();
}

int
cql_future_wait_timed(
   CqlFuture* future,
    size_t            wait) {
  return static_cast<int>(
      future->wait_for(wait));
}

CqlSession*
cql_future_get_session(
   CqlFuture* future) {
  CqlSessionFutureImpl* session_future = static_cast<CqlSessionFutureImpl*>(future);
  return session_future->result;
}

CqlResult*
cql_future_get_result(
   CqlFuture* future) {
  CqlMessageFutureImpl* message_future = static_cast<CqlMessageFutureImpl*>(future);
  return static_cast<CqlResult*>(message_future->result->body.release());
}

CqlPrepared*
cql_future_get_prepare(
   CqlFuture* future) {
  CqlMessageFutureImpl* message_future = static_cast<CqlMessageFutureImpl*>(future);
  CqlResult*            result         = static_cast<CqlResult*>(message_future->result->body.get());
  return new CqlPrepared(std::string(result->prepared, result->prepared_size));
}

void
cql_future_error_string(
    CqlFuture* future,
    char*      output,
    size_t     output_len,
    size_t*    total) {
  *total = future->error->message.copy(output, output_len);
}

int
cql_future_error_source(
    CqlFuture* future) {
  return future->error->source;
}

int
cql_future_error_code(
    CqlFuture* future) {
  return future->error->code;
}

const char*
cql_error_desc(
    int code) {
  return "";
}

int
cql_session_query(
    CqlSession*    session,
    const char*    statement,
    size_t         statement_length,
    size_t         paramater_count,
    CqlConsistency consistency,
    CqlStatement** output) {
  (void) session;
  *output = new CqlQueryStatement(paramater_count, consistency);
  (*output)->statement(statement, statement_length);
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_prepare(
    CqlSession* session,
    const char* statement,
    size_t      statement_length,
    CqlFuture** output) {
  *output = reinterpret_cast<CqlFuture*>(session->prepare(statement, statement_length));
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_bind(
    CqlSession*    session,
    CqlPrepared*   prepared,
    size_t         paramater_count,
    CqlConsistency consistency,
    CqlStatement** output) {
  *output = new CqlBoundStatement(*prepared, consistency);
  return CQL_ERROR_NO_ERROR;
}

int
cql_session_batch(
    CqlSession*         session,
    CqlConsistency      consistency,
    CqlBatchStatement** output) {
  *output = new CqlBatchStatement(consistency);
  return CQL_ERROR_NO_ERROR;
}

int
cql_batch_add_statement(
    CqlBatchStatement* batch,
    CqlStatement*      statement) {
  batch->add_statement(statement);
  return CQL_ERROR_NO_ERROR;
}

/**
 * Bind a short to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return
 */
int
cql_statement_bind_short(
    CqlStatement* statement,
    size_t        index,
    int16_t       value) {
  return statement->bind(index, value);
}

/**
 * Bind an int to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return
 */
int
cql_statement_bind_int(
    CqlStatement* statement,
    size_t        index,
    int32_t       value) {
  return statement->bind(index, value);
}

/**
 * Bind a bigint to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return
 */
int
cql_statement_bind_bigint(
    CqlStatement* statement,
    size_t        index,
    int64_t       value) {
  return statement->bind(index, value);
}

/**
 * Bind a float to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return
 */
int
cql_statement_bind_float(
    CqlStatement* statement,
    size_t        index,
    float         value) {
  return statement->bind(index, value);
}

/**
 * Bind a double to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_double(
    CqlStatement*  statement,
    size_t         index,
    double         value) {
  return statement->bind(index, value);
}

/**
 * Bind a bool to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_bool(
    CqlStatement*  statement,
    size_t         index,
    cql_bool       value) {
  return statement->bind(index, value);
}

/**
 * Bind a time stamp to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_time(
    CqlStatement*  statement,
    size_t         index,
    int64_t        value) {
  return statement->bind(index, value);
}

/**
 * Bind a UUID to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_uuid(
    CqlStatement*  statement,
    size_t         index,
    CqlUuid        value) {
  return statement->bind(index, reinterpret_cast<char*>(value), sizeof(CqlUuid));
}

/**
 * Bind a counter to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_counter(
    CqlStatement*  statement,
    size_t         index,
    int64_t        value) {
  return statement->bind(index, value);
}

/**
 * Bind a string to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 * @param length
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_string(
    CqlStatement*  statement,
    size_t         index,
    char*          value,
    size_t         length) {
  return statement->bind(index, value, length);
}

/**
 * Bind a blob to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 * @param length
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_blob(
    CqlStatement*  statement,
    size_t         index,
    uint8_t*       value,
    size_t         length) {
  return statement->bind(index, reinterpret_cast<char*>(value), length);
}

/**
 * Bind a decimal to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param scale
 * @param value
 * @param length
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_decimal(
    CqlStatement* statement,
    size_t        index,
    uint32_t      scale,
    uint8_t*      value,
    size_t        length) {
  return CQL_ERROR_NO_ERROR;
}

/**
 * Bind a varint to a query or bound statement at the specified index
 *
 * @param statement
 * @param index
 * @param value
 * @param length
 *
 * @return NULL if successful, otherwise pointer to CqlError structure
 */
int
cql_statement_bind_varint(
    CqlStatement*  statement,
    size_t         index,
    uint8_t*       value,
    size_t         length) {
  return statement->bind(index, reinterpret_cast<char*>(value), length);
}

struct CqlLoadBalancingPolicy : public LoadBalancingPolicy {
  void* data_;
  std::vector<CqlHost> hosts_;
  CqlLoadBalancingPolicyImpl* impl_;

  CqlLoadBalancingPolicy(void* data, CqlLoadBalancingPolicyImpl* impl)
    : data_(data)
    , impl_(impl)  { }

  virtual void init(const std::vector<CqlHost>& hosts) {
    hosts_ = hosts;
    if(impl_->init_func != NULL) {
      impl_->init_func(this);
    }
  }

  virtual CqlHostDistance distance(const CqlHost& host) {
    if(impl_->host_distance_func != NULL) {
      return impl_->host_distance_func(this, &host);
    }
    return CQL_HOST_DISTANCE_IGNORE;
  }

  virtual void new_query_plan(std::list<CqlHost>* output) {
    //if(impl_->next_host_func != NULL) {
    //  const char* address = impl_->next_host_func(this, 1);
    //  while(address != NULL) {
    //    output->push_back(address);
    //    address = impl_->next_host_func(this, 0);
    //  }
    //}
  }
};

size_t cql_lb_policy_hosts_count(CqlLoadBalancingPolicy* policy) {
  return policy->hosts_.size();
}

CqlHost* cql_lb_policy_get_host(CqlLoadBalancingPolicy* policy, size_t index) {
  return &policy->hosts_[index];
}

void* cql_lb_policy_get_data(CqlLoadBalancingPolicy* policy) {
  return policy->data_;
}

const char* cql_host_get_address(CqlHost* host) {
  return host->address.to_string().c_str();
}

void cql_session_set_load_balancing_policy(CqlSession* session,
                                           void* data,
                                           CqlLoadBalancingPolicyImpl* impl) {
  session->set_load_balancing_policy(new CqlLoadBalancingPolicy(data, impl));
}

} // extern "C"
