#pragma once

#include "task.hpp"

namespace hf {

using Graph = std::vector<std::unique_ptr<Node>>;

/** 
@class FlowBuilder

@brief Building blocks of a task dependency graph.

*/
class FlowBuilder {

  friend class Heteroflow;
  friend class Executor;

  public:

    /**
    @brief creates a placeholder task

    @tparam T task type (PullTask, PushTask, KernelTask, and HostTask)

    @return task handle of type T
    */
    template <typename T>
    T placeholder();
    
    /**
    @brief creates a host task from a given callable object
    
    @tparam C callable type
    
    @param callable a callable object acceptable to std::function

    @return HostTask handle
    */
    template <typename C>
    HostTask host(C&& callable); 
    
    /**
    @brief creates a pull task that copies a given host memory block to gpu

    @tparam ArgsT argements types
    @param args arguments to forward to construct a pull task

    @return PullTask handle
    */
    template <typename... ArgsT>
    PullTask pull(ArgsT&&... args);
    
    /**
    @brief creates a push task that copies a given gpu memory block to the host
    
    @tparam ArgsT argements types
    @param args arguments to forward to construct a push task

    @return PushTask handle
    */
    template <typename... ArgsT>
    PushTask push(ArgsT&&... args);
    
    /**
    @brief creates a kernel task that launches a given kernel 
    
    @tparam ArgsT argements types
    @param args arguments to forward to construct a kernel task

    @return KernelTask handle
    */
    template <typename... ArgsT>
    KernelTask kernel(ArgsT&&... args);

    /**
    @brief creates a transfer task that send data between pull tasks
    
    @tparam ArgsT argements types
    @param args arguments to forward to construct a transfer task
    
    @return TransferTask handle
    */
    template <typename... ArgsT>
    TransferTask transfer(ArgsT&&... args);

    /**
    @brief clears the graph
    */
    void clear();
    
    /**
    @brief queries the number of nodes
    */
    size_t num_nodes() const;
    
    /**
    @brief queries the number of nodes
    */
    bool empty() const;


  private:

    Graph _graph;
};

// Function: placeholder
template <typename T>
T FlowBuilder::placeholder() {
  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<typename T::node_handle_t>{}
  ));
  return T(_graph.back().get());
}

// Function: host
template <typename C>
HostTask FlowBuilder::host(C&& callable) {

  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<Node::Host>{}
  ));
  
  // assign the work
  return HostTask(_graph.back().get())
        .work(std::forward<C>(callable));
}

// Function: pull
template <typename... C>
PullTask FlowBuilder::pull(C&&... c) {
  
  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<Node::Pull>{}
  ));

  return PullTask(_graph.back().get())
        .pull(std::forward<C>(c)...);
}

// Function: push
template <typename... ArgsT>
PushTask FlowBuilder::push(ArgsT&&... args) {

  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<Node::Push>{}
  ));

  return PushTask(_graph.back().get())
        .push(std::forward<ArgsT>(args)...);
}


// Function: transfer 
template <typename... ArgsT>
TransferTask FlowBuilder::transfer(ArgsT&&... args) {

  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<Node::Transfer>{}
  ));

  return TransferTask(_graph.back().get())
        .transfer(std::forward<ArgsT>(args)...);
}


// Function: kernel    
template <typename... ArgsT>
KernelTask FlowBuilder::kernel(ArgsT&&... args) {
  
  _graph.emplace_back(std::make_unique<Node>(
    nonstd::in_place_type_t<Node::Kernel>{}
  ));

  return KernelTask(_graph.back().get())
        .kernel(std::forward<ArgsT>(args)...);
}

// Procedure: clear
inline void FlowBuilder::clear() {
  _graph.clear();
}

// Function: num_nodes
inline size_t FlowBuilder::num_nodes() const {
  return _graph.size();
}

// Function: empty
inline bool FlowBuilder::empty() const {
  return _graph.empty();
}

}  // end of namespace hf -----------------------------------------------------


