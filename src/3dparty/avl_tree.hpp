//    avl-tree
//    Copyright (C) 2015 -- 2016  Jonas Greitemann
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

/**
 * From https://github.com/jgreitemann/avl-tree, modified.
 */

#pragma once
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <memory>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <type_traits>

namespace AVL {
  template<typename T, typename LessComparator = std::less<T>, typename A = std::allocator<T> >
  class tree {
    class EqComparator {
     public:
      bool operator()(const T &lhs, const T &rhs) const {
          return !m_cmp(lhs, rhs) && !m_cmp(rhs, lhs);
      }
     private:
      LessComparator m_cmp;
    };

   public:
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

   private:
    class node {
     public:
      T data{}; // todo remove requirement of defult ctor
      short depth;
      size_type n;
      node *parent;
      node *left_child;
      node *right_child;

      node() noexcept
          : depth(1), n(1), parent(nullptr), left_child(nullptr), right_child(nullptr) {}

      explicit node(const T &t) noexcept
          : data(t), depth(1), n(1), parent(nullptr), left_child(nullptr), right_child(nullptr) {}

      explicit node(T &&t) noexcept
          : data(std::move(t)), depth(1), n(1), parent(nullptr), left_child(nullptr), right_child(nullptr) {}

      #ifdef DEBUGMODE
      void print(std::ostream& os, std::string prefix) {
          os << "(" << data << "," << n << ","
          << depth << ")" << std::endl;
          if (!left_child && !right_child)
          return;
          if (left_child) {
          os << prefix << "+---";
          left_child->print(os, prefix + "|   ");
          } else {
          os << prefix << "+" << std::endl;
          }
          if (right_child) {
          os << prefix << "+---";
          right_child->print(os, prefix + "    ");
          } else {
          os << prefix << "+" << std::endl;
          }
      }
      #endif

      void update_depth() {
          depth = 1 + std::max(left_child ? left_child->depth : 0,
                               right_child ? right_child->depth : 0);
      }

      void update_n() {
          n = 1 + (left_child ? left_child->n : 0)
              + (right_child ? right_child->n : 0);
      }

      short imbalance() {
          return (right_child ? right_child->depth : 0)
              - (left_child ? left_child->depth : 0);
      }
    };

   public:
    class const_iterator;

    class iterator {
      friend class tree;
      friend class const_iterator;
//      template<typename U, typename V>
//      friend
//      class tree<U, V>::const_iterator;
     public:
      typedef typename A::difference_type difference_type;
      typedef typename A::value_type value_type;
      typedef typename A::reference reference;
      typedef typename A::pointer pointer;
      typedef std::bidirectional_iterator_tag iterator_category;

      iterator() {
          m_ptr = nullptr;
      }

      explicit iterator(node *p) {
          m_ptr = p;
      }

      iterator(const iterator &it) {
          m_ptr = it.m_ptr;
      }

      iterator &operator=(const iterator &it) {
          m_ptr = it.m_ptr;
          return *this;
      }

      bool operator==(const iterator &it) const {
          return m_ptr == it.m_ptr;
      }

      bool operator!=(const iterator &it) const {
          return m_ptr != it.m_ptr;
      }

      bool operator<(const iterator &it) const {
          return m_cmp(**this, *it);
      }

      bool operator>(const iterator &it) const {
          return *it < **this;
      }

      bool operator<=(const iterator &it) const {
          return **this <= *it;
      }

      bool operator>=(const iterator &it) const {
          return **this >= *it;
      }

      // pre-increment
      iterator &operator++() {
          if (m_ptr->right_child) {
              m_ptr = m_ptr->right_child;
              while (m_ptr->left_child) {
                  m_ptr = m_ptr->left_child;
              }
          } else {
              node *before;
              do {
                  before = m_ptr;
                  m_ptr = m_ptr->parent;
              } while (m_ptr && before == m_ptr->right_child);
          }
          return *this;
      }

      // post-increment
      iterator operator++(int) {
          iterator old(*this);
          ++(*this);
          return old;
      }

      // pre-decrement
      iterator &operator--() {
          if (m_ptr->left_child) {
              m_ptr = m_ptr->left_child;
              while (m_ptr->right_child) {
                  m_ptr = m_ptr->right_child;
              }
          } else {
              node *before;
              do {
                  before = m_ptr;
                  m_ptr = m_ptr->parent;
              } while (m_ptr && before == m_ptr->left_child);
          }
          return *this;
      }

      // post-decrement
      iterator operator--(int) {
          iterator old(*this);
          --(*this);
          return old;
      }

      reference operator*() const {
          return m_ptr->data;
      }

      pointer operator->() const {
          return &(m_ptr->data);
      }
     private:
      node *m_ptr;
      LessComparator m_cmp;
    };

    class const_iterator {
     public:
      typedef typename A::difference_type difference_type;
      typedef typename A::value_type value_type;
      typedef typename A::const_reference const_reference;
      typedef typename A::const_pointer const_pointer;
      typedef std::bidirectional_iterator_tag iterator_category;

      const_iterator() {
          m_ptr = nullptr;
      }

      explicit const_iterator(const node *p) {
          m_ptr = p;
      }

      const_iterator(const const_iterator &it) {
          m_ptr = it.m_ptr;
      }

      explicit const_iterator(const iterator &it) {
          m_ptr = it.m_ptr;
      }

      const_iterator &operator=(const const_iterator &it) {
          m_ptr = it.m_ptr;
          return *this;
      }

      bool operator==(const const_iterator &it) const {
          return m_ptr == it.m_ptr;
      }

      bool operator!=(const const_iterator &it) const {
          return m_ptr != it.m_ptr;
      }

      bool operator<(const const_iterator &it) const {
          return m_cmp(**this, *it);
      }

      bool operator>(const const_iterator &it) const {
          return *it < **this;
      }

      bool operator<=(const const_iterator &it) const {
          return **this <= *it;
      }

      bool operator>=(const const_iterator &it) const {
          return **this >= *it;
      }

      // pre-increment
      const_iterator &operator++() {
          if (m_ptr->right_child) {
              m_ptr = m_ptr->right_child;
              while (m_ptr->left_child) {
                  m_ptr = m_ptr->left_child;
              }
          } else {
              const node *before;
              do {
                  before = m_ptr;
                  m_ptr = m_ptr->parent;
              } while (m_ptr && before == m_ptr->right_child);
          }
          return *this;
      }

      // post-increment
      const_iterator operator++(int) {
          const_iterator old(*this);
          ++(*this);
          return old;
      }

      // pre-decrement
      const_iterator &operator--() {
          if (m_ptr->left_child) {
              m_ptr = m_ptr->left_child;
              while (m_ptr->right_child) {
                  m_ptr = m_ptr->right_child;
              }
          } else {
              const node *before;
              do {
                  before = m_ptr;
                  m_ptr = m_ptr->parent;
              } while (m_ptr && before == m_ptr->left_child);
          }
          return *this;
      }

      // post-decrement
      const_iterator operator--(int) {
          const_iterator old(*this);
          --(*this);
          return old;
      }

      const_reference operator*() const {
          return (const_reference) (m_ptr->data);
      }

      const_pointer operator->() const {
          return &(m_ptr->data);
      }
     private:
      node const *m_ptr;
      LessComparator m_cmp;
    };

    tree() noexcept {
        m_root = m_alloc.allocate(1);
        m_alloc.construct(m_root);
        m_root->n = 0;
    }

    tree(const tree &t) noexcept {
        *this = t;
    }

    tree(tree &&t) noexcept {
        m_root = t.m_root;
        t.m_root = m_alloc.allocate(1);
        m_alloc.construct(t.m_root);
        t.m_root->n = 0;
    }

    ~tree() noexcept {
        clear_node(m_root);
        m_alloc.destroy(m_root);
        m_alloc.deallocate(m_root, 1);
    }

    tree &operator=(const tree &t) noexcept {
        m_root = deep_copy_node(t.m_root);
        return *this;
    }

    tree &operator=(tree &&t) noexcept {
        clear();
        std::swap(m_root, t.m_root);
    }

    bool operator==(const tree &t) const {
        const_iterator it1, it2;
        for (it1 = cbegin(), it2 = t.cbegin();
             it1 != cend() && it2 != t.cend();
             ++it1, ++it2) {
            if (*it1 != *it2)
                return false;
        }
        if (it1 == cend() && it2 == t.cend()) {
            return true;
        } else {
            return false;
        }
    }

    bool operator!=(const tree &t) const {
        return !(*this == t);
    }

    iterator begin() {
        node *ptr = m_root;
        while (ptr->left_child) {
            ptr = ptr->left_child;
        }
        return iterator(ptr);
    }

    const_iterator begin() const {
        const node *ptr = m_root;
        while (ptr->left_child) {
            ptr = ptr->left_child;
        }
        return const_iterator(ptr);
    }

    const_iterator cbegin() const {
        const node *ptr = m_root;
        while (ptr->left_child) {
            ptr = ptr->left_child;
        }
        return const_iterator(ptr);
    }

    iterator end() {
        return iterator(m_root);
    }

    const_iterator end() const {
        return const_iterator(m_root);
    }

    const_iterator cend() const {
        return const_iterator(m_root);
    }

    reference front() {
        iterator b = begin();
        return *b;
    }

    const_reference front() const {
        const_iterator b = begin();
        return *b;
    }

    reference back() {
        iterator b = end();
        return *(--b);
    }

    const_reference back() const {
        const_iterator b = end();
        return *(--b);
    }

    iterator insert(const T &t) {
        iterator res;

        // descent the search tree
        node *parent = m_root;
        while (true) {
            ++parent->n;
            if (parent == m_root || m_less(t, parent->data)) {
                if (parent->left_child) {
                    parent = parent->left_child;
                } else {
                    parent->left_child = m_alloc.allocate(1);
                    m_alloc.construct(parent->left_child, t);
                    parent->left_child->parent = parent;
                    res = iterator(parent->left_child);
                    break;
                }
            } else {
                if (parent->right_child) {
                    parent = parent->right_child;
                } else {
                    parent->right_child = m_alloc.allocate(1);
                    m_alloc.construct(parent->right_child, t);
                    parent->right_child->parent = parent;
                    res = iterator(parent->right_child);
                    break;
                }
            }
        }
        short branch_depth = 1;
        do {
            if (parent->depth > branch_depth)
                break;
            parent->depth = 1 + branch_depth;
            if (parent == m_root)
                break;
            if (parent->imbalance() < -1) {
                // check for double-rotation case
                if (parent->left_child->imbalance() > 0) {
                    rotate_left(parent->left_child);
                }
                rotate_right(parent);
                break;
            } else if (parent->imbalance() > 1) {
                // check for double-rotation case
                if (parent->right_child->imbalance() < 0) {
                    rotate_right(parent->right_child);
                }
                rotate_left(parent);
                break;
            }
            branch_depth = parent->depth;
            parent = parent->parent;
        } while (parent);
        return res;
    }

    iterator insert(T &&t) {
        iterator res;

        // descent the search tree
        node *parent = m_root;
        while (true) {
            ++parent->n;
            if (parent == m_root || m_less(t, parent->data)) {
                if (parent->left_child) {
                    parent = parent->left_child;
                } else {
                    parent->left_child = m_alloc.allocate(1);
                    m_alloc.construct(parent->left_child, std::move(t));
                    parent->left_child->parent = parent;
                    res = iterator(parent->left_child);
                    break;
                }
            } else {
                if (parent->right_child) {
                    parent = parent->right_child;
                } else {
                    parent->right_child = m_alloc.allocate(1);
                    m_alloc.construct(parent->right_child, std::move(t));
                    parent->right_child->parent = parent;
                    res = iterator(parent->right_child);
                    break;
                }
            }
        }
        short branch_depth = 1;
        do {
            if (parent->depth > branch_depth)
                break;
            parent->depth = 1 + branch_depth;
            if (parent == m_root)
                break;
            if (parent->imbalance() < -1) {
                // check for double-rotation case
                if (parent->left_child->imbalance() > 0) {
                    rotate_left(parent->left_child);
                }
                rotate_right(parent);
                break;
            } else if (parent->imbalance() > 1) {
                // check for double-rotation case
                if (parent->right_child->imbalance() < 0) {
                    rotate_right(parent->right_child);
                }
                rotate_left(parent);
                break;
            }
            branch_depth = parent->depth;
            parent = parent->parent;
        } while (parent);
        return res;
    }

    iterator at(size_type i) {
        // bounds checking
        if (i >= size()) {
            throw std::out_of_range("tree::at out-of-range");
        }

        size_type j = i;
        node *ptr = m_root->left_child;
        while (true) {
            if (ptr->left_child) {
                if (j == ptr->left_child->n) {
                    break;
                } else if (j < ptr->left_child->n) {
                    ptr = ptr->left_child;
                } else {
                    j -= 1 + ptr->left_child->n;
                    ptr = ptr->right_child;
                }
            } else {
                if (j == 0) {
                    break;
                } else {
                    --j;
                    ptr = ptr->right_child;
                }
            }
        }
        return iterator(ptr);
    }

    const_iterator at(size_type i) const {
        // bounds checking
        if (i >= size()) {
            throw std::out_of_range("tree[] out-of-range");
        }

        size_type j = i;
        const node *ptr = m_root->left_child;
        while (true) {
            if (ptr->left_child) {
                if (j == ptr->left_child->n) {
                    break;
                } else if (j < ptr->left_child->n) {
                    ptr = ptr->left_child;
                } else {
                    j -= 1 + ptr->left_child->n;
                    ptr = ptr->right_child;
                }
            } else {
                if (j == 0) {
                    break;
                } else {
                    --j;
                    ptr = ptr->right_child;
                }
            }
        }
        return const_iterator(ptr);
    }

    reference operator[](size_type i) {
        return *at(i);
    }

    const_reference operator[](size_type i) const {
        return *at(i);
    }

    iterator erase(iterator it) {
        iterator itn(it);
        ++itn;
        node *ptr = it.m_ptr;
        node *q;
        if (!ptr->left_child || !ptr->right_child) {
            q = ptr;
        } else {
            q = itn.m_ptr;
        }
        node *s;
        if (q->left_child) {
            s = q->left_child;
            q->left_child = nullptr;
        } else {
            s = q->right_child;
            q->right_child = nullptr;
        }
        if (s) {
            s->parent = q->parent;
        }
        if (q == q->parent->left_child) {
            q->parent->left_child = s;
        } else {
            q->parent->right_child = s;
        }
        node *q_parent = q->parent;
        if (q != ptr) {
            q->parent = ptr->parent;
            if (q->parent->left_child == ptr) {
                q->parent->left_child = q;
            } else {
                q->parent->right_child = q;
            }
            q->left_child = ptr->left_child;
            if (q->left_child)
                q->left_child->parent = q;
            q->right_child = ptr->right_child;
            if (q->right_child)
                q->right_child->parent = q;
            q->n = ptr->n;
            q->depth = ptr->depth;
            ptr->left_child = nullptr;
            ptr->right_child = nullptr;
        }
        if (q_parent == ptr) {
            q_parent = q;
        }
        node *parent;
        for (parent = q_parent; parent; parent = parent->parent) {
            --parent->n;
        }
        for (parent = q_parent; parent; parent = parent->parent) {
            parent->update_depth();
            if (parent == m_root)
                break;
            if (parent->imbalance() < -1) {
                // check for double-rotation case
                if (parent->left_child->imbalance() > 0) {
                    rotate_left(parent->left_child);
                }
                rotate_right(parent);
                break;
            } else if (parent->imbalance() > 1) {
                // check for double-rotation case
                if (parent->right_child->imbalance() < 0) {
                    rotate_right(parent->right_child);
                }
                rotate_left(parent);
                break;
            }
        }
        m_alloc.destroy(ptr);
        m_alloc.deallocate(ptr, 1);
        return itn;
    }

    iterator find(const_reference t) {
        node *ptr = m_root->left_child;
        while (ptr) {
            if (m_eq(t, ptr->data)) {
                return iterator(ptr);
            } else if (m_less(t, ptr->data)) {
                ptr = ptr->left_child;
            } else {
                ptr = ptr->right_child;
            }
        }
        return end();
    }

    const_iterator find(const_reference t) const {
        node *ptr = m_root->left_child;
        while (ptr) {
            if (m_eq(t, ptr->data)) {
                return const_iterator(ptr);
            } else if (m_less(t, ptr->data)) {
                ptr = ptr->left_child;
            } else {
                ptr = ptr->right_child;
            }
        }
        return cend();
    }

    void remove(const_reference t) {
        iterator it = find(t);
        if (it == end())
            return;
        do {
            it = erase(it);
        } while (m_eq(*it, t));
    }

    void clear() noexcept {
        clear_node(m_root);
        m_root->left_child = nullptr;
        m_root->n = 0;
        m_root->depth = 1;
    }

    void swap(tree &t) {
        std::swap(m_root, t.m_root);
    }

    size_type size() const {
        return m_root->n;
    }

    size_type max_size();

    bool empty() const {
        return m_root->left_child == nullptr;
    }

    A get_allocator() {
        return m_alloc;
    }

   private:
    void rotate_left(node *n) {
        node *tmp = n->right_child->left_child;
        if (n == n->parent->left_child) {
            n->parent->left_child = n->right_child;
        } else {
            n->parent->right_child = n->right_child;
        }
        n->right_child->parent = n->parent;
        n->right_child->left_child = n;
        n->parent = n->right_child;
        n->right_child = tmp;
        if (tmp)
            tmp->parent = n;

        // update ns
        n->update_n();
        n->parent->update_n();

        // update depths
        do {
            n->update_depth();
            n = n->parent;
        } while (n);
    }

    void rotate_right(node *n) {
        node *tmp = n->left_child->right_child;
        if (n == n->parent->left_child) {
            n->parent->left_child = n->left_child;
        } else {
            n->parent->right_child = n->left_child;
        }
        n->left_child->parent = n->parent;
        n->left_child->right_child = n;
        n->parent = n->left_child;
        n->left_child = tmp;
        if (tmp)
            tmp->parent = n;

        // update ns
        n->update_n();
        n->parent->update_n();

        // update depths
        do {
            n->update_depth();
            n = n->parent;
        } while (n);
    }

    node *deep_copy_node(const node *nd) {
        node *cp_nd = m_alloc.allocate(1);
        m_alloc.construct(cp_nd, nd->data);
        cp_nd->n = nd->n;
        cp_nd->depth = nd->depth;
        if (nd->left_child) {
            cp_nd->left_child = deep_copy_node(nd->left_child);
            cp_nd->left_child->parent = cp_nd;
        }
        if (nd->right_child) {
            cp_nd->right_child = deep_copy_node(nd->right_child);
            cp_nd->right_child->parent = cp_nd;
        }
        return cp_nd;
    }

    void clear_node(node *nd) noexcept {
        if (nd->left_child) {
            clear_node(nd->left_child);
            m_alloc.destroy(nd->left_child);
            m_alloc.deallocate(nd->left_child, 1);
        }
        if (nd->right_child) {
            clear_node(nd->right_child);
            m_alloc.destroy(nd->right_child);
            m_alloc.deallocate(nd->right_child, 1);
        }
    }

    using NodeAlloc = typename std::allocator_traits<A>::template rebind_alloc<node>;
    NodeAlloc m_alloc;
    node *m_root;
    LessComparator m_less;
    EqComparator m_eq;

    #ifdef DEBUGMODE
    template <typename U, typename V>
    friend std::ostream& operator<< (std::ostream&, const tree<U,V>&);
    #endif
  }; // class tree

  template<typename T, typename A = std::allocator<T> >
  void swap(tree<T, A> &t1, tree<T, A> &t2) {
      t1.swap(t2);
  }

#ifdef DEBUGMODE
  template <typename T, typename A = std::allocator<T> >
  std::ostream& operator<<(std::ostream& os, const tree<T,A>& t) {
      if (!t.m_root->left_child)
          return os << "(empty)" << std::endl;
      t.m_root->left_child->print(os, "");
      return os;
  }
#endif

}
