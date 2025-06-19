# 🚀 CS2 DX11 Injection Base

> A lightweight & modular DLL injection base for **Counter-Strike 2**, built with **DirectX 11**, **ImGui**, and **MinHook** — supports memory-loaded fonts and modular menu design.

---

## ✨ Features

- 🎯 **CS2 Compatible** – Designed to inject smoothly into Counter-Strike 2.
- ⚙️ **DirectX 11 Hooking** – Hooks `IDXGISwapChain::Present` for rendering overlays.
- 🧩 **Modular Structure** – Clean separation of Renderer, Hook, and Menu modules.
- 🎨 **ImGui Interface** – Custom ImGui-based menu with full keyboard/mouse capture.
- 📝 **Memory Font Loading** – Supports loading fonts from memory (no external font files).
- 🪝 **MinHook Integrated** – Simple and effective function hooking using [MinHook](https://github.com/TsudaKageyu/minhook).

---

## 🗂️ Project Structure

Base/
├── dllmain.cpp # Entry point
├── Hook/ # Hook logic (e.g., Present)
├── Renderer/ # ImGui DX11 rendering
├── Library/ # External libs (ImGui, MinHook, etc.)
---

## 🧪 Hotkeys

| Key      | Action                 |
|----------|------------------------|
| `F5`     | Toggle menu            |
| `END`    | Uninject & clean up    |

---

## 🖼️ Screenshot

> *(Add a screenshot here)*
![image](https://github.com/user-attachments/assets/5c49a850-f4a7-42d5-ac05-b3e6923ff9a0)

---

## 🛠️ Dependencies

- **ImGui** `v1.89+` (with DX11 + Win32 backends)
- **MinHook**
- **DirectX 11**

---

## 📦 How to Build

1. Clone repository & open `Base.sln` in Visual Studio 2022+  
2. Set build mode to `Release | x64`
3. Build the DLL
4. Inject into CS2 using your injector

---

## ⚠️ Disclaimer

> This project is provided **for educational purposes only**.  
> Do **not** use it to gain unfair advantages in multiplayer games.

---

## 📜 License

This project is licensed under the **MIT License**.  
See [`LICENSE`](./LICENSE) for more details.

---
