import streamlit as st
import pandas as pd

# Page config — sabse pehle set karna ZARURI hai, koi aur st call se pehle
st.set_page_config(
    page_title="SCAE — Smart City Algorithm Engine",
    page_icon="🏙️",
    layout="wide",
    initial_sidebar_state="expanded"
)

# ==============================================================
# Sidebar navigation — mentor ko clear structure dikhao
# ==============================================================
with st.sidebar:
    st.title("🏙️ SCAE Navigation")
    st.caption("Tanishka Gupta | 24022809")
    st.divider()

    st.page_link("app.py",                    label="🏠 Home")
    st.page_link("pages/01_search.py",        label="🔍 Search Algorithms")
    st.page_link("pages/02_sort.py",          label="📊 Sort Benchmark")
    st.page_link("pages/03_graph.py",         label="🗺️ Graph & Navigation")
    st.page_link("pages/04_shortest_paths.py",label="🚨 Shortest Paths")
    st.page_link("pages/05_mst.py",           label="⚡ MST — Power Grid")
    st.page_link("pages/06_greedy.py",        label="📅 Greedy Scheduler")
    st.page_link("pages/07_dp.py",            label="🎒 Knapsack & DP")
    st.page_link("pages/08_huffman_lcs.py",   label="🗜️ Huffman & LCS")
    st.page_link("pages/09_hashing.py",       label="🔑 Hash Tables")
    st.page_link("pages/10_string.py",        label="📝 String Search")
    st.page_link("pages/11_np_lab.py",        label="🧩 NP-Complete Lab")

# ==============================================================
# Main page — landing screen
# ==============================================================
st.title("🏙️ Smart City Algorithm Engine")
st.subheader("35 Algorithms | Python + C Backend | Streamlit Dashboard")

st.markdown(
    """
    SCAE is a live, interactive demonstration of **35 classical algorithms** applied to
    real-world Smart City problems — GPS navigation, power grid optimization, citizen lookup,
    and more. Every algorithm runs in a native **C shared library (scae.dll)** and is bridged
    to this dashboard via Python **ctypes**, giving you true, hardware-level performance metrics
    right inside your browser.
    """
)

st.divider()

# ==============================================================
# Metrics — quick overview for first impression
# ==============================================================
col1, col2, col3, col4, col5 = st.columns(5)
col1.metric("Algorithms",  "35")
col2.metric("Units",       "5")
col3.metric("Test Cases",  "177+")
col4.metric("Backend",     "C + DLL")
col5.metric("Bridge",      "ctypes")

st.divider()

# ==============================================================
# Algorithm table — poori portfolio ek jagah
# ==============================================================
st.subheader("📋 Algorithm Portfolio")

data = {
    "Unit": [
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "1 — Search & Recursion",
        "2 — Sorting",
        "2 — Sorting",
        "2 — Sorting",
        "2 — Sorting",
        "2 — Sorting",
        "2 — Sorting",
        "2 — Sorting",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "3 — Graph Algorithms",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "4 — Greedy & DP",
        "5 — Hashing & String & NP",
        "5 — Hashing & String & NP",
        "5 — Hashing & String & NP",
        "5 — Hashing & String & NP",
    ],
    "Algorithm": [
        "Binary Search (Iterative)",
        "Binary Search (Recursive)",
        "Linear Search",
        "Linear Search (Sentinel)",
        "Tower of Hanoi",
        "Fibonacci (Naive)",
        "Fibonacci (Memoized)",
        "Bubble Sort",
        "Insertion Sort",
        "Selection Sort",
        "Counting Sort",
        "Merge Sort",
        "Quick Sort",
        "Heap Sort",
        "BFS Traversal",
        "BFS Shortest Path",
        "DFS Traversal",
        "Cycle Detection",
        "Connected Components",
        "Dijkstra",
        "Bellman-Ford",
        "Floyd-Warshall",
        "Kruskal MST",
        "Prim MST",
        "Activity Selection",
        "Job Sequencing",
        "Fractional Knapsack",
        "0/1 Knapsack",
        "Huffman Encoding",
        "LCS",
        "Matrix Chain Order",
        "Hash Table (Chaining)",
        "Hash Table (Open Addr.)",
        "Naive + KMP + Rabin-Karp",
        "TSP + Subset Sum",
    ],
    "Complexity": [
        "O(log n)", "O(log n)", "O(n)", "O(n)",
        "O(2ⁿ)", "O(2ⁿ)", "O(n)",
        "O(n²)", "O(n²)", "O(n²)",
        "O(n+k)", "O(n log n)", "O(n log n)", "O(n log n)",
        "O(V+E)", "O(V+E)", "O(V+E)", "O(V+E)",
        "O(V+E)", "O(V²)", "O(VE)", "O(V³)",
        "O(E log E)", "O(V²)",
        "O(n log n)", "O(n²)", "O(n log n)",
        "O(nW)", "O(n log n)", "O(mn)", "O(n³)",
        "O(1) avg", "O(1) avg",
        "O(nm)/O(n+m)", "O(n!)/O(nW)",
    ],
    "Module": [
        "01_search", "01_search", "01_search", "01_search",
        "01_search", "01_search", "01_search",
        "02_sort", "02_sort", "02_sort",
        "02_sort", "02_sort", "02_sort", "02_sort",
        "03_graph", "03_graph", "03_graph", "03_graph",
        "03_graph", "04_shortest_paths", "04_shortest_paths",
        "04_shortest_paths", "05_mst", "05_mst",
        "06_greedy", "06_greedy", "06_greedy",
        "07_dp", "08_huffman_lcs", "08_huffman_lcs",
        "08_huffman_lcs",
        "09_hashing", "09_hashing",
        "10_string", "11_np_lab",
    ],
}

df = pd.DataFrame(data)
st.dataframe(
    df,
    use_container_width=True,
    hide_index=True,
    column_config={
        "Unit":       st.column_config.TextColumn("Unit", width="medium"),
        "Algorithm":  st.column_config.TextColumn("Algorithm", width="large"),
        "Complexity": st.column_config.TextColumn("Complexity", width="small"),
        "Module":     st.column_config.TextColumn("Page Module", width="medium"),
    }
)

st.info("👈 Select a module from the sidebar to begin!")
