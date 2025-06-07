
# 🕷️ Parallel Web Crawler with Wikipedia Shortest Path Game

## Overview

This project explores methods for **parallelizing and optimizing** a web crawler, with a specific focus on crawling **Wikipedia** pages and building a graph of internal links. The final application includes a game that finds the **shortest hyperlink path** between two Wikipedia pages.

We developed and benchmarked two versions of our crawler:
- `Crawler1`: A parallelized version of a sequential crawler.
- `Crawler2`: An optimized, multi-threaded crawler with separated responsibilities for **fetching** and **processing** pages.

---

## 🚀 Features

- ⚙️ **Multithreaded Crawling**: Fetch and process pages concurrently using thread-safe queues and data structures.
- 📉 **Efficient Performance**: Over **10× speedup** compared to the sequential version.
- 🌐 **Web Graph Construction**: Builds a graph from Wikipedia pages using hyperlinks as edges.
- 🎮 **Shortest Path Game**: Find the shortest hyperlink path between two given Wikipedia pages.

---

## 📊 Performance Summary

| Crawler Version       | Speedup vs Sequential |
|-----------------------|------------------------|
| Crawler1 (Naive)      | ~4×                    |
| Crawler2 (Optimized)  | **~10×**               |

Crawler2 achieves more than **2× the speed** of Crawler1 by dedicating threads specifically to **fetching** pages and batching fetch operations.

---

## How to Run

### Build Instructions

* To compile the entire project:

  ```
  make
  ```

* To compile a specific main file:

  ```
  make build/{main_file_name}
  ```

### Generating the Graph

`main1` and `main2` generate a `.dot` file (`web_graph.dot`) representing the graph structure.

To visualize this graph:

* Generate a PNG:

  ```
  sfdp -Goverlap=prism -Gsep=0.5 -Nfontsize=6 -Epenwidth=0.2 -Tpng web_graph.dot -o spaced_graph.png
  ```

* Generate an SVG:

  ```
  sfdp -Goverlap=prism -Gsep=0.5 -Nfontsize=6 -Epenwidth=0.2 -Tsvg web_graph.dot -o spaced_graph.svg
  ```

---

## Authors
Berra Dogan
Remi Guillou

Note: ChatGBT was used for documentation and utilizing new libraries (e.g. GraphViz).