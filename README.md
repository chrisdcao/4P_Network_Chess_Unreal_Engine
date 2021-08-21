# 4P Network Chess Unreal Engine ♟️
This repository contains the code I made for 4P Online chess using Unreal Engine 4.26. 

# Algorithms and Data Structures 💻
1. Hash Keys (custom data structure, like HashMap but w/o Values): specifically made for check mate function
2. Min Priority Queue: for collecting continuous movable indices of pieces
3. Heap sort: sorting the movables when needed
4. Bitwise XOR: for setting up the chess board

# Graphic Optimization 🧊
1. Custom Level of Details (4 LODs) created in Blender, each with custom distances for rendering
2. Instancing of static meshes: created board tiles using Instanced Static Mesh, saving GPU draw calls
3. Instancing of Materials: using Create Material Instance, reduce memory pre-loads of materials
