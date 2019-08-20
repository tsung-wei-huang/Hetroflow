#include <iostream>
#include <heteroflow/heteroflow.hpp>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/typeof/std/utility.hpp>

__global__ void simple(size_t Nx, size_t Ny) {
  printf(
    "Hello from block %d, thread %d (%lux%lu)\n", 
    blockIdx.x, threadIdx.x,
    Nx, Ny
  );
}

int main() {

  static_assert(hf::function_traits<decltype(simple)>::arity == 2);

  static_assert(std::is_function<decltype(simple)>::value);

  typedef BOOST_TYPEOF(simple) foo_type;;
  typedef boost::function_traits<foo_type> function_traits;

  std::cout << typeid(foo_type).name() << std::endl;
  std::cout << function_traits::arity << std::endl;
  std::cout << typeid(function_traits::arg1_type).name() << ",";
  std::cout << typeid(function_traits::arg2_type).name() << std::endl;
  

  //int count = 1;
  //
  //auto ret = ::cudaSetDevice(count);

  //HF_CHECK_CUDA(ret, "can't set device");

  //::cudaGetDevice(&count);

  //std::cout << count << std::endl;

  float* data = new float [100];
  size_t N = 100;

  hf::Heteroflow hf;

  auto p0 = hf.placeholder<hf::KernelTask>();
  auto p1 = hf.host([](){});
  auto p2 = hf.pull(data, N);
  auto p3 = hf.push(data, p2, N);
  auto p4 = hf.kernel(simple, 8, 2); 

  //simple<<<2, 2>>>(8, 2);
  p4.grid({1, 2, 3});
  p4.grid_x(10);
  std::cout << p4.grid_x() << std::endl;
  std::cout << p4.grid_y() << std::endl;
  std::cout << p4.grid_z() << std::endl;
  p4.block({1, 2, 3});
  p4.block_x(10);
  std::cout << p4.block_x() << std::endl;
  std::cout << p4.block_y() << std::endl;
  std::cout << p4.block_z() << std::endl;

  cudaDeviceSynchronize();

  //fb2.insert(p1) 

  //assert(p1 && p2);

  //p1.precede(p2);


  //std::cout << t1.num_dependents() << " " 
  //          << t1.num_successors() << std::endl;

  //HF_THROW_IF(h_X == nullptr, "f---", h_X)
  //HF_CHECK_CUDA(cudaErrorInitializationError, "succeFFFFFF", h_X);
  //success                            = cudaSuccess,
  //missing_configuration              = cudaErrorMissingConfiguration,
  //memory_allocation                  = cudaErrorMemoryAllocation,
  //initialization_error               = cudaErrorInitializationError,
  
  // create a heteroflow
  /*hf::Heteroflow hf;

  auto new_X = hf.host([&](){ h_X = new float [32]; });
  auto new_Y = hf.host([&](){ h_Y = new float [64]; });
  auto gpu_X = hf.pull(h_X, n_X);
  auto gpu_Y = hf.pull(h_Y, n_Y);

  // kernel task (depends on gpu_X and gpu_Y)
  auto kernel = hf.kernel(simple, gpu_X, 32, gpu_Y, 64);

  auto push_X = hf.push(h_X, gpu_X, n_X);
  auto push_Y = hf.push(h_Y, gpu_Y, n_Y);
  auto kill_X = hf.host([&](){ delete [] h_X; });
  auto kill_Y = hf.host([&](){ delete [] h_Y; });
  
  // build up the dependency
  new_X.precede(gpu_X);
  new_Y.precede(gpu_Y);
  gpu_X.precede(kernel);
  gpu_Y.precede(kernel);
  kernel.precede(push_X, push_Y);
  push_X.precede(kill_X);
  push_Y.precede(kill_Y);
  
  // dump the heteroflow graph
  hf.dump(std::cout);
  
  // create an executor
  hf::Executor executor;
  executor.run(hf);
*/
  return 0;
}





