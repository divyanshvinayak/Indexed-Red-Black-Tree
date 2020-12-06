/*
* Copyright (c) 2020 Divyansh Vinayak
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef INDEXED_RED_BLACK_TREE_HPP
#define INDEXED_RED_BLACK_TREE_HPP

#include <cstddef>

namespace __rb_tree
{
	template <typename T>
	class indexed_red_black_tree
	{
		enum Color
		{
			RED,
			BLACK
		};

		struct Node
		{
			T data;
			size_t size;
			Color color;
			Node *left, *right, *parent;
		};

		Node *root, *null;

		T t = T();

		Node *Search(size_t pos)
		{
			Node *temp = root;
			size_t idx = temp->left->size + 1;
			while (idx != pos)
			{
				if (pos < idx)
				{
					temp = temp->left;
				}
				else
				{
					pos -= idx;
					temp = temp->right;
				}
				idx = temp->left->size + 1;
			}
			return temp;
		}

		void RotateLeft(Node *node)
		{
			Node *parent = node->right;
			node->right = parent->left;
			if (parent->left != null)
			{
				parent->left->parent = node;
			}
			parent->parent = node->parent;
			if (node->parent == NULL)
			{
				root = parent;
			}
			else if (node == node->parent->left)
			{
				node->parent->left = parent;
			}
			else
			{
				node->parent->right = parent;
			}
			parent->left = node;
			node->parent = parent;
			parent->size = node->size;
			node->size = node->left->size + node->right->size + 1;
		}

		void RotateRight(Node *node)
		{
			Node *parent = node->left;
			node->left = parent->right;
			if (parent->right != null)
			{
				parent->right->parent = node;
			}
			parent->parent = node->parent;
			if (node->parent == NULL)
			{
				root = parent;
			}
			else if (node == node->parent->right)
			{
				node->parent->right = parent;
			}
			else
			{
				node->parent->left = parent;
			}
			parent->right = node;
			node->parent = parent;
			parent->size = node->size;
			node->size = node->left->size + node->right->size + 1;
		}

		void FixInsert(Node *node)
		{
			Node *uncle;
			while (node->parent->color == RED)
			{
				if (node->parent == node->parent->parent->right)
				{
					uncle = node->parent->parent->left;
					if (uncle->color == RED)
					{
						uncle->color = BLACK;
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						node = node->parent->parent;
					}
					else
					{
						if (node == node->parent->left)
						{
							node = node->parent;
							RotateRight(node);
						}
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						RotateLeft(node->parent->parent);
					}
				}
				else
				{
					uncle = node->parent->parent->right;
					if (uncle->color == RED)
					{
						uncle->color = BLACK;
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						node = node->parent->parent;
					}
					else
					{
						if (node == node->parent->right)
						{
							node = node->parent;
							RotateLeft(node);
						}
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						RotateRight(node->parent->parent);
					}
				}
				if (node == root)
				{
					break;
				}
			}
			root->color = BLACK;
		}

		void Insert(size_t pos, T data)
		{
			Node *node = new Node{data, 1, RED, null, null, NULL}, *temp;
			if (root == null)
			{
				root = node;
				node->color = BLACK;
				return;
			}
			else
			{
				if (pos == root->size + 1)
				{
					temp = root;
					while (temp->right != null)
					{
						temp->size++;
						temp = temp->right;
					}
					temp->size++;
					temp->right = node;
					node->parent = temp;
				}
				else
				{
					temp = root;
					size_t idx = temp->left->size + 1;
					while (idx != pos)
					{
						temp->size++;
						if (pos < idx)
						{
							temp = temp->left;
						}
						else
						{
							pos -= idx;
							temp = temp->right;
						}
						idx = temp->left->size + 1;
					}
					temp->size++;
					if (temp->left == null)
					{
						temp->left = node;
						node->parent = temp;
					}
					else
					{
						temp = temp->left;
						while (temp->right != null)
						{
							temp->size++;
							temp = temp->right;
						}
						temp->size++;
						temp->right = node;
						node->parent = temp;
					}
				}
			}
			if (node->parent == root)
			{
				return;
			}
			FixInsert(node);
		}

		Node *BSTReplace(Node *node)
		{
			node->size--;
			if (node->left != null && node->right != null)
			{
				node = node->right;
				while (node->left != null)
				{
					node->size--;
					node = node->left;
				}
				return node;
			}
			if (node->left == null && node->right == null)
			{
				return null;
			}
			if (node->left != null)
			{
				return node->left;
			}
			return node->right;
		}

		Node *Sibling(Node *node)
		{
			if (node == root)
			{
				return null;
			}
			if (node == node->parent->left)
			{
				return node->parent->right;
			}
			return node->parent->left;
		}

		void FixDoubleBlack(Node *node)
		{
			if (node == root)
			{
				return;
			}
			Node *parent = node->parent, *sibling = Sibling(node);
			if (sibling == null)
			{
				FixDoubleBlack(parent);
			}
			else
			{
				if (sibling->color == RED)
				{
					parent->color = RED;
					sibling->color = BLACK;
					if (sibling == parent->left)
					{
						RotateRight(parent);
					}
					else
					{
						RotateLeft(parent);
					}
					FixDoubleBlack(node);
				}
				else
				{
					if (sibling->left->color == RED || sibling->right->color == RED)
					{
						if (sibling->left->color == RED)
						{
							if (sibling == parent->left)
							{
								sibling->left->color = sibling->color;
								sibling->color = parent->color;
								RotateRight(parent);
							}
							else
							{
								sibling->left->color = parent->color;
								RotateRight(sibling);
								RotateLeft(parent);
							}
						}
						else
						{
							if (sibling == parent->left)
							{
								sibling->right->color = parent->color;
								RotateLeft(sibling);
								RotateRight(parent);
							}
							else
							{
								sibling->right->color = sibling->color;
								sibling->color = parent->color;
								RotateLeft(parent);
							}
						}
						parent->color = BLACK;
					}
					else
					{
						sibling->color = RED;
						if (parent->color == BLACK)
						{
							FixDoubleBlack(parent);
						}
						else
						{
							parent->color = BLACK;
						}
					}
				}
			}
		}

		void Delete(Node *node)
		{
			Node *temp = BSTReplace(node), *parent = node->parent, *sibling = Sibling(node);
			bool doubleblack = temp->color == BLACK && node->color == BLACK;
			if (temp == null)
			{
				if (node == root)
				{
					root = null;
				}
				else
				{
					if (doubleblack)
					{
						FixDoubleBlack(node);
					}
					else if (sibling != null)
					{
						sibling->color = RED;
					}
					if (node == parent->left)
					{
						parent->left = null;
					}
					else
					{
						parent->right = null;
					}
				}
				delete node;
				return;
			}
			if (node->left == null || node->right == null)
			{
				if (node == root)
				{
					node->data = temp->data;
					node->left = node->right = null;
					delete temp;
				}
				else
				{
					if (node == parent->left)
					{
						parent->left = temp;
					}
					else
					{
						parent->right = temp;
					}
					delete node;
					temp->parent = parent;
					if (doubleblack)
					{
						FixDoubleBlack(temp);
					}
					else
					{
						temp->color = BLACK;
					}
				}
				return;
			}
			node->data = temp->data;
			Delete(temp);
		}

		Node *Minimum(Node *node)
		{
			while (node->left != null)
			{
				node = node->left;
			}
			return node;
		}

		Node *Maximum(Node *node)
		{
			while (node->right != null)
			{
				node = node->right;
			}
			return node;
		}

		Node *Successor(Node *node)
		{
			if (node->right != null)
			{
				return Minimum(node->right);
			}
			Node *parent = node->parent;
			while (parent != null && node == parent->right)
			{
				node = parent;
				parent = parent->parent;
			}
			return parent;
		}

		Node *Predecessor(Node *node)
		{
			if (node->left != null)
			{
				return Maximum(node->left);
			}
			Node *parent = node->parent;
			while (parent != null && node == parent->left)
			{
				node = parent;
				parent = parent->parent;
			}
			return parent;
		}

		void Clear(Node *node)
		{
			if (node == null)
			{
				return;
			}
			Clear(node->left);
			Clear(node->right);
			delete node;
		}

	public:
		indexed_red_black_tree()
		{
			null = new Node{t, 0, BLACK, NULL, NULL, NULL};
			root = null;
		}

		void insert(size_t pos, T data)
		{
			Insert(pos, data);
		}

		void erase(size_t pos)
		{
			Node *temp = root;
			size_t idx = temp->left->size + 1;
			while (idx != pos)
			{
				temp->size--;
				if (pos < idx)
				{
					temp = temp->left;
				}
				else
				{
					pos -= idx;
					temp = temp->right;
				}
				idx = temp->left->size + 1;
			}
			Delete(temp);
		}

		void erase(Node *node)
		{
			Node *temp = node->parent;
			while (temp != NULL)
			{
				temp->size--;
				temp = temp->parent;
			}
			Delete(node);
		}

		Node *find(size_t pos)
		{
			return Search(pos);
		}

		T &at(size_t pos)
		{
			return Search(pos)->data;
		}

		T &operator[](size_t pos)
		{
			return Search(pos)->data;
		}

		Node *next(Node *node)
		{
			return Successor(node);
		}

		Node *prev(Node *node)
		{
			return Predecessor(node);
		}

		size_t size()
		{
			return root->size;
		}

		bool empty()
		{
			return root == null;
		}

		void clear()
		{
			Clear(root);
			root = null;
		}
	};
} // namespace __rb_tree

#endif