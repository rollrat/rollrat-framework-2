//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BinaryTree.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BINARYTREE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _BINARYTREE_9bf1541fdf7efd41b7b39543fd870ac4_

namespace ofw {

template <typename data_type>
class BinaryTreeNode {
 public:
  typedef class BinaryTreeNode<data_type> this_type;

  typedef typename data_type* pointer;
  typedef typename const data_type* const_pointer;
  typedef typename data_type& reference;
  typedef typename const data_type& const_reference;
  typedef typename data_type value_type;

  BinaryTreeNode()
      : _Left(nullptr),
        _Right(nullptr),
        _Parent(nullptr),
        _Data(value_type()) {}

  BinaryTreeNode(value_type _data)
      : _Left(nullptr), _Right(nullptr), _Parent(nullptr), _Data(_data) {}

  virtual reference operator*() { return this->_Data; }

  virtual bool is_left() { return _Left != nullptr; }
  virtual bool is_right() { return _Right != nullptr; }
  virtual bool is_parent() { return _Parent != nullptr; }
  // using nullptr cause annoying problem
  virtual bool is_data() { return _Data != value_type(); }

  this_type *_Left, *_Right, *_Parent;
  value_type _Data;
};

template <typename data_type>
class BinaryTree {
 public:
  typedef BinaryTree<data_type> this_type;
  typedef BinaryTreeNode<data_type> node_type;

  typedef typename node_type::pointer pointer;
  typedef typename node_type::const_pointer const_pointer;
  typedef typename node_type::reference reference;
  typedef typename node_type::const_reference const_reference;
  typedef typename node_type::value_type value_type;

  typedef typename node_type* node_pointer;
  typedef typename node_type& node_reference;
  typedef typename node_type*& node_pointer_reference;

  BinaryTree(bool safe_node = false)
      : attach_temp(nullptr),
        last_into(0),
        last_escape(0),
        safe_node(safe_node) {
    this->_Last = this->_Head = this->allocate();
  }

  BinaryTree(const BinaryTree& tree) { copy_travel(tree._Head); }

  explicit BinaryTree(node_pointer _Head) : BinaryTree() {
    this->_Last = this->_Head = _Head;
  }

  ~BinaryTree() {
    if (!safe_node) {
      delete_travel();
    }
  }

 protected:
  bool safe_node;

  node_pointer _Head, _Last;
  node_pointer attach_temp;

  //
  //    -1: left, 1: right, 0: head
  //
  int last_into;
  int last_escape;

 private:
  node_pointer allocate(const void* = 0) { return new node_type(); }

  void deallocate(node_pointer __p) { delete __p; }

  template <typename T>
  void swap(T& left, T& right) {
    T tmp = left;
    left = right;
    right = tmp;
  }

 public:
  void push_left() {
    node_pointer _Left = node_pointer();
    _Left = allocate();

    if (_Left) {
      **_Left = value_type();
      this->_Last->_Left = _Left;
      this->_Last->_Left->_Parent = this->_Last;
    } else {
      this->push_left();
    }
  }

  void push_right() {
    node_pointer _Right = node_pointer();
    _Right = allocate();

    if (_Right) {
      **_Right = value_type();
      this->_Last->_Right = _Right;
      this->_Last->_Right->_Parent = this->_Last;
    } else {
      this->push_right();
    }
  }

  void push_left(value_type value) {
    this->push_left();
    **this->_Last->_Left = value;
  }

  void push_right(value_type value) {
    this->push_right();
    **this->_Last->_Right = value;
  }

  bool into_left() {
    if ((*this->_Last).is_left())
      this->_Last = this->_Last->_Left;
    else
      return false;
    last_into = -1;
    return true;
  }

  bool into_right() {
    if ((*this->_Last).is_right())
      this->_Last = this->_Last->_Right;
    else
      return false;
    last_into = 1;
    return true;
  }

  void into_depp_left() {
    for (; into_left();)
      ;
  }

  void into_depp_right() {
    for (; into_right();)
      ;
  }

  bool escape() {
    if ((*this->_Last).is_parent()) {
      node_pointer chkd = this->_Last;
      this->_Last = this->_Last->_Parent;

      if (chkd == this->_Last->_Left)
        last_escape = -1;
      else if (chkd == this->_Last->_Right)
        last_escape = 1;
      else if (this->_Last == this->_Head)
        last_escape = 0;
    } else {
      return false;
    }

    return true;
  }

  void ground() { this->_Last = this->_Head; }

  bool is_left() { return ((*this->_Last).is_left()); }
  bool is_right() { return ((*this->_Last).is_right()); }
  bool is_data() { return ((*this->_Last).is_data()); }
  bool is_parent() { return ((*this->_Last).is_parent()); }
  bool is_ground() { return this->_Last == this->_Head; }

  reference operator*() { return **this->_Last; }
  node_pointer operator&() { return this->_Last; }
  pointer operator->() { return &(**this->_Last); }
  node_pointer travel() const { return this->_Head; }
  reference left() { return **this->_Last->_Left; }
  reference right() { return **this->_Last->_Right; }

  void make_head_left() {
    node_pointer _Head = node_pointer();
    _Head = allocate();
    **_Head = value_type();
    _Head->_Left = this->_Head;
    this->_Head->_Parent = _Head;
    this->_Head = _Head;
    this->_Last = this->_Head;
  }

  void make_head_right() {
    node_pointer _Head = node_pointer();
    _Head = allocate();
    **_Head = value_type();
    _Head->_Right = this->_Head;
    this->_Head->_Parent = _Head;
    this->_Head = _Head;
    this->_Last = this->_Head;
  }

  void make_head_left(value_type value) {
    this->make_head_left();
    **this->_Head = value;
  }

  void make_head_right(value_type value) {
    this->make_head_right();
    **this->_Head = value;
  }

  void attach_left() {
    if (!(*this->_Last).is_left() && this->attach_temp != nullptr) {
      this->_Last->_Left = attach_temp;
      this->_Last->_Left->_Parent = this->_Last;
      attach_temp = nullptr;
    }
  }

  void attach_right() {
    if (!(*this->_Last).is_right() && this->attach_temp != nullptr) {
      this->_Last->_Right = attach_temp;
      this->_Last->_Right->_Parent = this->_Last;
      attach_temp = nullptr;
    }
  }

  void attach_to_head() {
    if (this->attach_temp != nullptr) {
      this->_Last = this->_Head = attach_temp;
      attach_temp = nullptr;
    }
  }

  void detach_left() {
    if ((*this->_Last).is_left() && this->attach_temp == nullptr) {
      this->attach_temp = this->_Last->_Left;
      this->attach_temp->_Parent = nullptr;
      this->_Last->_Left = nullptr;
    }
  }

  void detach_right() {
    if ((*this->_Last).is_right() && this->attach_temp == nullptr) {
      this->attach_temp = this->_Last->_Right;
      this->attach_temp->_Parent = nullptr;
      this->_Last->_Right = nullptr;
    }
  }

  void detach_this() {
    if (this->is_ground()) {
      if (this->is_left() || this->is_right() || this->is_data()) {
        attach_temp = this->_Head;
        last_escape = last_into = 0;
        this->_Head = this->_Last = this->allocate();
      }
    } else {
      this->escape();
      if (last_escape == -1) {
        detach_left();
      } else if (last_escape == 1) {
        detach_right();
      }
    }
  }

  int last_detach() { return last_escape; }

  void attach_left_tree(node_pointer v_det) {
    if (!(*this->_Last).is_left()) {
      this->_Last->_Left = v_det;
      this->_Last->_Left->_Parent = this->_Last;
    }
  }

  void attach_right_tree(node_pointer v_det) {
    if (!(*this->_Last).is_right()) {
      this->_Last->_Right = v_det;
      this->_Last->_Right->_Parent = this->_Last;
    }
  }

  node_pointer_reference detach_left_tree() {
    if ((*this->_Last).is_left()) {
      node_pointer tmp = this->_Last->_Left;
      tmp->_Parent = nullptr;
      this->_Last->_Left = nullptr;
      return tmp;
    }
  }

  node_pointer_reference detach_right_tree() {
    if ((*this->_Last).is_right()) {
      node_pointer tmp = this->_Last->_Right;
      tmp->_Parent = nullptr;
      this->_Last->_Right = nullptr;
      return tmp;
    }
  }

  void swap_data_left() {
    if ((*this->_Last).is_left())
      if ((this->_Last->_Left)->is_data() && (this->_Last)->is_data())
        this->swap(**this->_Last, **this->_Last->_Left);
  }

  void swap_data_right() {
    if ((*this->_Last).is_right())
      if ((this->_Last->_Right)->is_data() && (this->_Last)->is_data())
        this->swap(**this->_Last, **this->_Last->_Right);
  }

  void swap_data_each() {
    if ((*this->_Last).is_left() && (*this->_Last).is_right())
      if ((this->_Last->_Right)->is_data() && (this->_Last->_Left)->is_data())
        this->swap(**this->_Last->_Left, **this->_Last->_Right);
  }

  template <class function_type>
  void postorder_travel(function_type& function, node_pointer _Next) {
    if (_Next == nullptr) return;
    postorder_travel(function, _Next->_Left);
    postorder_travel(function, _Next->_Right);
    function(_Next->_Data);
  }

  template <class function_type>
  void preorder_travel(function_type& function, node_pointer _Next) {
    if (_Next == nullptr) return;
    function(_Next->_Data);
    postorder_travel(function, _Next->_Left);
    postorder_travel(function, _Next->_Right);
  }

  template <class function_type>
  void inorder_travel(function_type& function, node_pointer _Next) {
    if (_Next == nullptr) return;
    postorder_travel(function, _Next->_Left);
    function(_Next->_Data);
    postorder_travel(function, _Next->_Right);
  }

  template <class function_type>
  void postorder_travel(function_type& function) {
    this->postorder_travel(function, this->travel());
  }

  template <class function_type>
  void preorder_travel(function_type& function) {
    this->preorder_travel(function, this->travel());
  }

  template <class function_type>
  void inorder_travel(function_type& function) {
    this->inorder_travel(function, this->travel());
  }

 private:
  void delete_travel(node_pointer _Next) {
    if (_Next == nullptr) return;
    delete_travel(_Next->_Left);
    delete_travel(_Next->_Right);
    deallocate(_Next);
  }

  void delete_travel() { delete_travel(this->travel()); }

  node_pointer copy_travel_i(node_pointer _Target) {
    node_pointer _Node = nullptr;
    if (_Target != nullptr) {
      _Node = node_pointer();
      _Node = allocate();
      _Node->_Data = _Target->_Data;
      _Node->_Left = copy_travel_i(_Target->_Left);
      _Node->_Right = copy_travel_i(_Target->_Right);
    }
    return _Node;
  }

  void copy_travel(node_pointer _Target) {
    this->_Head = this->_Last = copy_travel_i(_Target);
  }
};

}  // namespace ofw

#endif