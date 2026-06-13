import ctypes
import os
import sys

# DLL load karo — agar nahi mila toh helpful error do
_dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "scae.dll"))

try:
    scae_dll = ctypes.CDLL(_dll_path)
except FileNotFoundError:
    print(f"Error: scae.dll not found at {_dll_path}")
    print("Please run 'mingw32-make' in the parent directory first!")
    sys.exit(1)
except OSError as e:
    print(f"Error loading scae.dll at {_dll_path}: {e}")
    sys.exit(1)

# AlgoResult struct — C se exactly match karna ZARURI hai
class AlgoResult(ctypes.Structure):
    _fields_ = [
        ("output",      ctypes.POINTER(ctypes.c_int)),
        ("outputSize",  ctypes.c_int),
        ("steps",       ctypes.c_long),
        ("time_ns",     ctypes.c_double),
        ("big_o",       ctypes.c_char * 20),
        ("memory_used", ctypes.c_long),
        ("status",      ctypes.c_char * 50),
    ]

def _to_c_array(py_list):
    # Python list → ctypes int array
    return (ctypes.c_int * len(py_list))(*py_list)

def _result_to_dict(r: AlgoResult) -> dict:
    # AlgoResult → clean Python dict
    result_dict = {
        "output": [r.output[i] for i in range(r.outputSize)] if bool(r.output) else [],
        "outputSize": r.outputSize,
        "steps": r.steps,
        "time_ns": r.time_ns,
        "time_ms": r.time_ns / 1_000_000.0,
        "big_o": r.big_o.decode('utf-8') if r.big_o else "",
        "memory_used": r.memory_used,
        "status": r.status.decode('utf-8') if r.status else ""
    }
    # Note: Memory leak expected here because free_result is static inline in C
    # and cannot be called from Python ctypes directly.
    return result_dict

# ==============================================================
# Unit 1 - Search
# ==============================================================

scae_dll.binary_search_iterative.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.binary_search_iterative.restype = AlgoResult

def binary_search_iterative(arr: list, key: int) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.binary_search_iterative(c_arr, len(arr), key)
    return _result_to_dict(r)

scae_dll.binary_search_recursive.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.binary_search_recursive.restype = AlgoResult

def binary_search_recursive(arr: list, key: int) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.binary_search_recursive(c_arr, len(arr), key)
    return _result_to_dict(r)

scae_dll.linear_search.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.linear_search.restype = AlgoResult

def linear_search(arr: list, key: int) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.linear_search(c_arr, len(arr), key)
    return _result_to_dict(r)

scae_dll.linear_search_sentinel.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.linear_search_sentinel.restype = AlgoResult

def linear_search_sentinel(arr: list, key: int) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.linear_search_sentinel(c_arr, len(arr), key)
    return _result_to_dict(r)

scae_dll.tower_hanoi.argtypes = [ctypes.c_int]
scae_dll.tower_hanoi.restype = AlgoResult

def tower_hanoi(n: int) -> dict:
    r = scae_dll.tower_hanoi(n)
    return _result_to_dict(r)

scae_dll.fibonacci_naive.argtypes = [ctypes.c_int]
scae_dll.fibonacci_naive.restype = AlgoResult

def fibonacci_naive(n: int) -> dict:
    r = scae_dll.fibonacci_naive(n)
    return _result_to_dict(r)

scae_dll.fibonacci_memo.argtypes = [ctypes.c_int]
scae_dll.fibonacci_memo.restype = AlgoResult

def fibonacci_memo(n: int) -> dict:
    r = scae_dll.fibonacci_memo(n)
    return _result_to_dict(r)

# ==============================================================
# Unit 2 - Sort
# ==============================================================

scae_dll.bubble_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.bubble_sort.restype = AlgoResult

def bubble_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.bubble_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.insertion_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.insertion_sort.restype = AlgoResult

def insertion_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.insertion_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.selection_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.selection_sort.restype = AlgoResult

def selection_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.selection_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.counting_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.counting_sort.restype = AlgoResult

def counting_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.counting_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.merge_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.merge_sort.restype = AlgoResult

def merge_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.merge_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.quick_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.quick_sort.restype = AlgoResult

def quick_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.quick_sort(c_arr, len(arr))
    return _result_to_dict(r)

scae_dll.heap_sort.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.heap_sort.restype = AlgoResult

def heap_sort(arr: list) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.heap_sort(c_arr, len(arr))
    return _result_to_dict(r)


# ==============================================================
# Unit 3 - Graph algorithms: adj matrix flat array pass karo
# ==============================================================

scae_dll.bfs_traversal.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.bfs_traversal.restype = AlgoResult

def bfs_traversal(adj_matrix: list, V: int, src: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.bfs_traversal(c_arr, V, src)
    return _result_to_dict(r)

scae_dll.bfs_shortest_path_hops.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int, ctypes.c_int]
scae_dll.bfs_shortest_path_hops.restype = AlgoResult

def bfs_shortest_path_hops(adj_matrix: list, V: int, src: int, dst: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.bfs_shortest_path_hops(c_arr, V, src, dst)
    return _result_to_dict(r)

scae_dll.dfs_traversal.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.dfs_traversal.restype = AlgoResult

def dfs_traversal(adj_matrix: list, V: int, src: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.dfs_traversal(c_arr, V, src)
    return _result_to_dict(r)

scae_dll.has_cycle.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.has_cycle.restype = AlgoResult

def has_cycle(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.has_cycle(c_arr, V)
    return _result_to_dict(r)

scae_dll.count_components.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.count_components.restype = AlgoResult

def count_components(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.count_components(c_arr, V)
    return _result_to_dict(r)

scae_dll.dijkstra.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int, ctypes.c_int]
scae_dll.dijkstra.restype = AlgoResult

def dijkstra(adj_matrix: list, V: int, src: int, dst: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.dijkstra(c_arr, V, src, dst)
    return _result_to_dict(r)

scae_dll.bellman_ford.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int, ctypes.c_int]
scae_dll.bellman_ford.restype = AlgoResult

def bellman_ford(adj_matrix: list, V: int, src: int, dst: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.bellman_ford(c_arr, V, src, dst)
    return _result_to_dict(r)

scae_dll.floyd_warshall.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.floyd_warshall.restype = AlgoResult

def floyd_warshall(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.floyd_warshall(c_arr, V)
    return _result_to_dict(r)

scae_dll.kruskal_mst.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.kruskal_mst.restype = AlgoResult

def kruskal_mst(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.kruskal_mst(c_arr, V)
    return _result_to_dict(r)

scae_dll.prim_mst.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.prim_mst.restype = AlgoResult

def prim_mst(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.prim_mst(c_arr, V)
    return _result_to_dict(r)

# ==============================================================
# Unit 4 - Greedy+DP: do lists pass karne ke liye dono ko
#           separately _to_c_array karo
# ==============================================================

scae_dll.activity_selection.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.activity_selection.restype = AlgoResult

def activity_selection(start: list, finish: list, n: int) -> dict:
    c_start = _to_c_array(start)
    c_finish = _to_c_array(finish)
    r = scae_dll.activity_selection(c_start, c_finish, n)
    return _result_to_dict(r)

scae_dll.job_sequencing.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.job_sequencing.restype = AlgoResult

def job_sequencing(profit: list, deadline: list, n: int) -> dict:
    c_profit = _to_c_array(profit)
    c_deadline = _to_c_array(deadline)
    r = scae_dll.job_sequencing(c_profit, c_deadline, n)
    return _result_to_dict(r)

scae_dll.knapsack_fractional.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.knapsack_fractional.restype = AlgoResult

def knapsack_fractional(weight: list, value: list, n: int, capacity: int) -> dict:
    c_weight = _to_c_array(weight)
    c_value = _to_c_array(value)
    r = scae_dll.knapsack_fractional(c_weight, c_value, n, capacity)
    return _result_to_dict(r)

scae_dll.knapsack_01.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.knapsack_01.restype = AlgoResult

def knapsack_01(weight: list, value: list, n: int, capacity: int) -> dict:
    c_weight = _to_c_array(weight)
    c_value = _to_c_array(value)
    r = scae_dll.knapsack_01(c_weight, c_value, n, capacity)
    return _result_to_dict(r)

scae_dll.huffman_encode.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.huffman_encode.restype = AlgoResult

def huffman_encode(freq: list, n: int) -> dict:
    c_freq = _to_c_array(freq)
    r = scae_dll.huffman_encode(c_freq, n)
    return _result_to_dict(r)

scae_dll.lcs.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.lcs.restype = AlgoResult

def lcs(seq1: list, seq2: list) -> dict:
    c_seq1 = _to_c_array(seq1)
    c_seq2 = _to_c_array(seq2)
    r = scae_dll.lcs(c_seq1, len(seq1), c_seq2, len(seq2))
    return _result_to_dict(r)

scae_dll.matrix_chain_order.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.matrix_chain_order.restype = AlgoResult

def matrix_chain_order(dims: list, n: int) -> dict:
    c_dims = _to_c_array(dims)
    r = scae_dll.matrix_chain_order(c_dims, n)
    return _result_to_dict(r)

# ==============================================================
# Unit 5 - String matching: text aur pattern dono convert karo
# ==============================================================

scae_dll.hash_chain_build.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.hash_chain_build.restype = AlgoResult

def hash_chain_build(keys: list, table_size: int) -> dict:
    c_keys = _to_c_array(keys)
    r = scae_dll.hash_chain_build(c_keys, len(keys), table_size)
    return _result_to_dict(r)

scae_dll.hash_chain_search.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int, ctypes.c_int]
scae_dll.hash_chain_search.restype = AlgoResult

def hash_chain_search(keys: list, table_size: int, search_key: int) -> dict:
    c_keys = _to_c_array(keys)
    r = scae_dll.hash_chain_search(c_keys, len(keys), table_size, search_key)
    return _result_to_dict(r)

scae_dll.hash_open_build.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.hash_open_build.restype = AlgoResult

def hash_open_build(keys: list, table_size: int) -> dict:
    c_keys = _to_c_array(keys)
    r = scae_dll.hash_open_build(c_keys, len(keys), table_size)
    return _result_to_dict(r)

scae_dll.hash_open_search.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int, ctypes.c_int]
scae_dll.hash_open_search.restype = AlgoResult

def hash_open_search(keys: list, table_size: int, search_key: int) -> dict:
    c_keys = _to_c_array(keys)
    r = scae_dll.hash_open_search(c_keys, len(keys), table_size, search_key)
    return _result_to_dict(r)

scae_dll.naive_string_match.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.naive_string_match.restype = AlgoResult

def naive_string_match(text: list, pattern: list) -> dict:
    c_text = _to_c_array(text)
    c_pattern = _to_c_array(pattern)
    r = scae_dll.naive_string_match(c_text, len(text), c_pattern, len(pattern))
    return _result_to_dict(r)

scae_dll.kmp_search.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.kmp_search.restype = AlgoResult

def kmp_search(text: list, pattern: list) -> dict:
    c_text = _to_c_array(text)
    c_pattern = _to_c_array(pattern)
    r = scae_dll.kmp_search(c_text, len(text), c_pattern, len(pattern))
    return _result_to_dict(r)

scae_dll.rabin_karp_search.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.rabin_karp_search.restype = AlgoResult

def rabin_karp_search(text: list, pattern: list) -> dict:
    c_text = _to_c_array(text)
    c_pattern = _to_c_array(pattern)
    r = scae_dll.rabin_karp_search(c_text, len(text), c_pattern, len(pattern))
    return _result_to_dict(r)

scae_dll.tsp_brute.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.tsp_brute.restype = AlgoResult

def tsp_brute(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.tsp_brute(c_arr, V)
    return _result_to_dict(r)

scae_dll.tsp_greedy.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
scae_dll.tsp_greedy.restype = AlgoResult

def tsp_greedy(adj_matrix: list, V: int) -> dict:
    c_arr = _to_c_array(adj_matrix)
    r = scae_dll.tsp_greedy(c_arr, V)
    return _result_to_dict(r)

scae_dll.subset_sum.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
scae_dll.subset_sum.restype = AlgoResult

def subset_sum(arr: list, target: int) -> dict:
    c_arr = _to_c_array(arr)
    r = scae_dll.subset_sum(c_arr, len(arr), target)
    return _result_to_dict(r)


if __name__ == "__main__":
    print("Testing bridge.py...")

    # Unit 1+2 tests (keep existing)
    r = binary_search_iterative([1, 3, 5, 7, 9], 5)
    assert r["status"] == "FOUND" and r["output"][0] == 2
    r2 = bubble_sort([5, 3, 1, 4, 2])
    assert r2["output"] == [1, 2, 3, 4, 5]
    print("Unit 1+2: OK")

    # Unit 3 tests
    # Linear graph 0-1-2-3 as flat 4x4 adjacency matrix
    adj = [0]*16
    adj[0*4+1]=1; adj[1*4+0]=1
    adj[1*4+2]=1; adj[2*4+1]=1
    adj[2*4+3]=1; adj[3*4+2]=1
    r3 = bfs_traversal(adj, 4, 0)
    assert r3["status"] == "SUCCESS" and r3["outputSize"] == 4
    r4 = dijkstra(adj, 4, 0, 3)
    assert r4["status"] == "SUCCESS"
    print("Unit 3: OK")

    # Unit 4 tests
    r5 = knapsack_01([1,3,4,5], [1,4,5,7], 4, 7)
    assert r5["output"][0] == 9
    r6 = lcs([1,3,4,5,6,7,8], [1,3,5,7,8,9,10])
    assert r6["output"][0] == 5
    print("Unit 4: OK")

    # Unit 5 tests
    r7 = kmp_search([1,2,3,2,1,2,3], [1,2,3])
    assert r7["status"] == "FOUND" and r7["outputSize"] == 2
    r8 = tsp_brute([0,1,1,1, 1,0,1,1, 1,1,0,1, 1,1,1,0], 4)
    assert r8["output"][0] == 4
    print("Unit 5: OK")

    print("\nbridge.py ALL UNITS: COMPLETE!")
