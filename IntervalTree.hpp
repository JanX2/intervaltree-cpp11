#ifndef INTERVALTREE
#define INTERVALTREE

#include <string>
#include <vector>
#include <iostream>

/////////////////////////////////////////////////////////////////////
// File Name               : IntervalTree.hpp
//      Created            : 08 11 2012   10:45
//      Authors            : Costin S, Jan Weiß
//
/////////////////////////////////////////////////////////////////////
#define TREE_WITH_PARENT_POINTERS

namespace IntervalTree {
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//	using namespace System;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//	using namespace System::Collections::Generic;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//	using namespace System::Diagnostics;

	/// <summary>
	/// Interval structure
	/// </summary>
	/// <typeparam name="T"></typeparam>
//C# TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
	template<typename T> /*where T : IComparable<T>*/
	class Interval {

	public:
		T Start;
		T End;



		Interval(T start, T end) : Interval() {
			if (start->compare(end) >= 0) {
				throw std::invalid_argument("the start value of the interval must be smaller than the end value. null interval are not allowed");
			}

			this->Start = start;
			this->End = end;
		}



		/// <summary>
		/// Determines if two intervals overlap (i.e. if this interval starts before the other ends and it finishes after the other starts)
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns>
		///   <c>true</c> if the specified other is overlapping; otherwise, <c>false</c>.
		/// </returns>
		bool OverlapsWith(Interval<T> other) {
			return this->Start->compare(other.End) < 0 && this->End->compare(other.Start) > 0;
		}

#if 0
		virtual std::string ToString() override {
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
			return std::string::Format("[{0}, {1}]", this->Start->ToString(), this->End->ToString());
		}
#endif

	};

	/// <summary>
	/// Interval Tree class
	/// </summary>
	/// <typeparam name="T"></typeparam>
//C# TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
	template<typename T, typename TypeValue> /* where T : IComparable<T> */
	class IntervalTree {
	public:
		typedef Interval<T*> IntervalType; // was IntervalTree::Interval<T*>
		typedef std::vector<IntervalType> IntervalVector;
		
		typedef std::pair<IntervalType*, TypeValue*> IntervalPair; // was KeyValuePair<IntervalType*, TypeValue*>
		typedef std::vector<IntervalPair*> IntervalPairVector;
		
		typedef std::pair<T*, TypeValue*> TypePair; // was KeyValuePair<T*, TypeValue*>
		typedef std::vector<TypePair*> TypePairVector; // was std::vector<typePair*>
	private:
		class IntervalNode {

#if defined(TREE_WITH_PARENT_POINTERS)
		private:
			IntervalNode *Parent;
#endif


			int privateBalance;
		public:
			const int &getBalance() const {
				return privateBalance;
			}
			void setBalance(const int &value) {
				privateBalance = value;
			}
		private:
			IntervalNode *privateLeft;
		public:
			const IntervalNode &getLeft() const {
				return privateLeft;
			}
			void setLeft(const IntervalNode &value) {
				privateLeft = value;
			}
		private:
			IntervalNode *privateRight;
		public:
			const IntervalNode &getRight() const {
				return privateRight;
			}
			void setRight(const IntervalNode &value) {
				privateRight = value;
			}
		private:
			IntervalType privateInterval;
		public:
			const IntervalType &getInterval() const {
				return privateInterval;
			}
			void setInterval(const IntervalType &value) {
				privateInterval = value;
			}
		private:
			TypeValue *privateValue;
		public:
			const TypeValue &getValue() const {
				return privateValue;
			}
			void setValue(const TypeValue &value) {
				privateValue = value;
			}
		private:
			TypePairVector privateRange;
		public:
			const TypePairVector &getRange() const {
				return privateRange;
			}
			void setRange(const TypePairVector &value) {
				privateRange = value;
			}
		private:
			T *privateMax;
		public:
			const T &getMax() const {
				return privateMax;
			}
			void setMax(const T &value) {
				privateMax = value;
			}



			IntervalNode(IntervalType interval, TypeValue *value) {
//C# TO C++ CONVERTER WARNING: C# to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
				delete this->getLeft();
//C# TO C++ CONVERTER WARNING: C# to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
				delete this->getRight();
				this->setBalance(0);
				this->setInterval(interval);
				this->setValue(value);
				this->setMax(interval.End);
			}



			/// <summary>
			/// Adds the specified elem.
			/// </summary>
			/// <param name="elem">The elem.</param>
			/// <param name="data">The data.</param>
			/// <returns></returns>
			static IntervalNode *Add(IntervalNode *elem, IntervalType interval, TypeValue *value, bool &wasAdded, bool &wasSuccessful) {
				if (elem == nullptr) {
					elem = new IntervalNode(interval, value);
					wasAdded = true;
					wasSuccessful = true;
				}
				else {
					int compareResult = interval.Start->compare(elem->getInterval().Start);
					IntervalNode *newChild = nullptr;
					if (compareResult < 0) {
						newChild = Add(elem->getLeft(), interval, value, wasAdded, wasSuccessful);
						if (elem->getLeft() != newChild) {
							elem->setLeft(newChild);
#if defined(TREE_WITH_PARENT_POINTERS)
							newChild->Parent = elem;
#endif
						}

						if (wasAdded) {
							elem->setBalance(elem->getBalance() - 1);

							if (elem->getBalance() == 0) {
								wasAdded = false;
							}
							else if (elem->getBalance() == -2) {
								if (elem->getLeft()->getBalance() == 1) {
									int elemLeftRightBalance = elem->getLeft()->getRight()->getBalance();

									elem->setLeft(RotateLeft(elem->getLeft()));
									elem = RotateRight(elem);

									elem->setBalance(0);
									elem->getLeft()->setBalance(elemLeftRightBalance == 1 ? - 1 : 0);
									elem->getRight()->setBalance(elemLeftRightBalance == -1 ? 1 : 0);
								}
								else if (elem->getLeft()->getBalance() == -1) {
									elem = RotateRight(elem);
									elem->setBalance(0);
									elem->getRight()->setBalance(0);
								}
								wasAdded = false;
							}
						}
					}
					else if (compareResult > 0) {
						newChild = Add(elem->getRight(), interval, value, wasAdded, wasSuccessful);
						if (elem->getRight() != newChild) {
							elem->setRight(newChild);
#if defined(TREE_WITH_PARENT_POINTERS)
							newChild->Parent = elem;
#endif
						}

						if (wasAdded) {
							elem->setBalance(elem->getBalance() + 1);
							if (elem->getBalance() == 0) {
								wasAdded = false;
							}
							else if (elem->getBalance() == 2) {
								if (elem->getRight()->getBalance() == -1) {
									int elemRightLeftBalance = elem->getRight()->getLeft()->getBalance();

									elem->setRight(RotateRight(elem->getRight()));
									elem = RotateLeft(elem);

									elem->setBalance(0);
									elem->getLeft()->setBalance(elemRightLeftBalance == 1 ? - 1 : 0);
									elem->getRight()->setBalance(elemRightLeftBalance == -1 ? 1 : 0);
								}

								else if (elem->getRight()->getBalance() == 1) {
									elem = RotateLeft(elem);

									elem->setBalance(0);
									elem->getLeft()->setBalance(0);
								}
								wasAdded = false;
							}
						}
					}
					else {
						//// if there are more than one interval starting at the same time/value, the intervalnode.Interval stores the start time and the maximum end time of all intervals starting at the same value.
						//// all end values (except the maximum end time/value which is stored in the interval node itself) are stored in the Range list in decreasing order.
						//// note: this is ok for problems where intervals starting at the same time /value is not a frequent occurrence, however you can use other data structure for better performance depending on your problem needs

						elem->AddIntervalValuePair(interval, value);

						wasSuccessful = true;
					}
					ComputeMax(elem);
				}

				return elem;
			}

			/// <summary>
			/// Computes the max.
			/// </summary>
			/// <param name="node">The node.</param>
			static void ComputeMax(IntervalNode *node) {
				T *maxRange = node->getInterval().End;

				if (node->getLeft() == nullptr && node->getRight() == nullptr) {
					node->setMax(maxRange);
				}
				else if (node->getLeft() == nullptr) {
					node->setMax((maxRange->compare(node->getRight()->getMax()) >= 0) ? maxRange : node->getRight()->getMax());
				}
				else if (node->getRight() == nullptr) {
					node->setMax((maxRange->compare(node->getLeft()->getMax()) >= 0) ? maxRange : node->getLeft()->getMax());
				}
				else {
					T *leftMax = node->getLeft()->getMax();
					T *rightMax = node->getRight()->getMax();

					if (leftMax->compare(rightMax) >= 0) {
						node->setMax(maxRange->compare(leftMax) >= 0 ? maxRange : leftMax);
					}
					else {
						node->setMax(maxRange->compare(rightMax) >= 0 ? maxRange : rightMax);
					}
				}
			}

			/// <summary>
			/// Finds the min.
			/// </summary>
			/// <param name="node">The node.</param>
			/// <returns></returns>
			static IntervalNode *FindMin(IntervalNode *node) {
				while (node != nullptr && node->getLeft() != nullptr) {
					node = node->getLeft();
				}
				return node;
			}

			/// <summary>
			/// Finds the max.
			/// </summary>
			/// <param name="node">The node.</param>
			/// <returns></returns>
			static IntervalNode *FindMax(IntervalNode *node) {
				while (node != nullptr && node->getRight() != nullptr) {
					node = node->getRight();
				}
				return node;
			}

			/// <summary>
			/// Gets the range of intervals stored in this.Range (i.e. starting at the same value 'this.Interval.Start' as the interval stored in the node itself)
			/// The range intervals are sorted in the descending order of their End interval values
			/// </summary>
			/// <returns></returns>
			IEnumerable<IntervalPair*> *GetRange() {
				if (this->getRange().size() > 0) {
					for (auto value : this->getRange()) {
						auto kth = IntervalType(this->getInterval().Start, value->first);
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
						yield return new IntervalPair(kth, value->Value);
					}
				}
				else {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
					yield break;
				}
			}

			/// <summary>
			/// Gets the range of intervals stored in this.Range (i.e. starting at the same value 'this.Interval.Start' as the interval stored in the node itself).
			/// The range intervals are sorted in the ascending order of their End interval values
			/// </summary>
			/// <returns></returns>
			IEnumerable<IntervalPair*> *GetRangeReverse() {
				if (this->getRange().size() > 0 && this->getRange().size() > 0) {
					int rangeCount = this->getRange().size();
					for (int k = rangeCount - 1; k >= 0; k--) {
						auto kth = IntervalType(this->getInterval().Start, this->getRange()[k]->first);
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
						yield return new IntervalPair(kth, this->getRange()[k]->Value);
					}
				}
				else {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
					yield break;
				}
			}

#if defined(TREE_WITH_PARENT_POINTERS)

			/// <summary>
			/// Succeeds this instance.
			/// </summary>
			/// <returns></returns>
			IntervalNode *Successor() {
				if (this->getRight() != nullptr)
					return FindMin(this->getRight());
				else {
					auto p = this;
					while (p->Parent != nullptr && p->Parent->Right == p) {
						p = p->Parent;
					}
					return p->Parent;
				}
			}

			/// <summary>
			/// Precedes this instance.
			/// </summary>
			/// <returns></returns>
			IntervalNode *Predecesor() {
				if (this->getLeft() != nullptr) {
					return FindMax(this->getLeft());
				}
				else {
					auto p = this;
					while (p->Parent != nullptr && p->Parent->Left == p) {
						p = p->Parent;
					}
					return p->Parent;
				}
			}
#endif

			/// <summary>
			/// Deletes the specified node.
			/// </summary>
			/// <param name="node">The node.</param>
			/// <param name="arg">The arg.</param>
			/// <returns></returns>
			static IntervalNode *Delete(IntervalNode *node, IntervalType arg, bool &wasDeleted, bool &wasSuccessful) {
				int cmp = arg.Start->compare(node->getInterval().Start);
				IntervalNode *newChild = nullptr;

				if (cmp < 0) {
					if (node->getLeft() != nullptr) {
						newChild = Delete(node->getLeft(), arg, wasDeleted, wasSuccessful);
						if (node->getLeft() != newChild) {
							node->setLeft(newChild);
						}

						if (wasDeleted) {
							node->setBalance(node->getBalance() + 1);
						}
					}
				}
				else if (cmp == 0) {
					if (arg.End->compare(node->getInterval().End) == 0 && node->getRange().empty()) {
						if (node->getLeft() != nullptr && node->getRight() != nullptr) {
							auto min = FindMin(node->getRight());

							auto interval = node->getInterval();
							node->Swap(min);

							wasDeleted = false;

							newChild = Delete(node->getRight(), interval, wasDeleted, wasSuccessful);
							if (node->getRight() != newChild) {
								node->setRight(newChild);
							}

							if (wasDeleted) {
								node->setBalance(node->getBalance() - 1);
							}
						}
						else if (node->getLeft() == nullptr) {
							wasDeleted = true;
							wasSuccessful = true;

#if defined(TREE_WITH_PARENT_POINTERS)
							if (node->getRight() != nullptr) {
								node->getRight()->Parent = node->Parent;
							}
#endif
							return node->getRight();
						}
						else {
							wasDeleted = true;
							wasSuccessful = true;
#if defined(TREE_WITH_PARENT_POINTERS)
							if (node->getLeft() != nullptr) {
								node->getLeft()->Parent = node->Parent;
							}
#endif
							return node->getLeft();
						}
					}
					else {
						wasSuccessful = node->DeleteIntervalFromNodeWithRange(arg);
					}
				}
				else {
					if (node->getRight() != nullptr) {
						newChild = Delete(node->getRight(), arg, wasDeleted, wasSuccessful);
						if (node->getRight() != newChild) {
							node->setRight(newChild);
						}

						if (wasDeleted) {
							node->setBalance(node->getBalance() - 1);
						}
					}
				}

				ComputeMax(node);

				if (wasDeleted) {
					if (node->getBalance() == 1 || node->getBalance() == -1) {
						wasDeleted = false;
						return node;
					}
					else if (node->getBalance() == -2) {
						if (node->getLeft()->getBalance() == 1) {
							int leftRightBalance = node->getLeft()->getRight()->getBalance();

							node->setLeft(RotateLeft(node->getLeft()));
							node = RotateRight(node);

							node->setBalance(0);
							node->getLeft()->setBalance((leftRightBalance == 1) ? - 1 : 0);
							node->getRight()->setBalance((leftRightBalance == -1) ? 1 : 0);
						}
						else if (node->getLeft()->getBalance() == -1) {
							node = RotateRight(node);
							node->setBalance(0);
							node->getRight()->setBalance(0);
						}
						else if (node->getLeft()->getBalance() == 0) {
							node = RotateRight(node);
							node->setBalance(1);
							node->getRight()->setBalance(-1);

							wasDeleted = false;
						}
					}
					else if (node->getBalance() == 2) {
						if (node->getRight()->getBalance() == -1) {
							int rightLeftBalance = node->getRight()->getLeft()->getBalance();

							node->setRight(RotateRight(node->getRight()));
							node = RotateLeft(node);

							node->setBalance(0);
							node->getLeft()->setBalance((rightLeftBalance == 1) ? - 1 : 0);
							node->getRight()->setBalance((rightLeftBalance == -1) ? 1 : 0);
						}
						else if (node->getRight()->getBalance() == 1) {
							node = RotateLeft(node);
							node->setBalance(0);
							node->getLeft()->setBalance(0);
						}
						else if (node->getRight()->getBalance() == 0) {
							node = RotateLeft(node);
							node->setBalance(-1);
							node->getLeft()->setBalance(1);

							wasDeleted = false;
						}
					}
				}

				return node;
			}

			/// <summary>
			/// Returns all intervals beginning at the specified start value. The intervals are sorted based on their End value (i.e. returned in ascending order of their End values)
			/// </summary>
			/// <param name="subtree">The subtree.</param>
			/// <param name="data">The data.</param>
			/// <returns></returns>
			static IntervalPairVector GetIntervalsStartingAt(IntervalNode *subtree, T *start) {
				if (subtree != nullptr) {
					int compareResult = start->compare(subtree->getInterval().Start);
					if (compareResult < 0) {
						return GetIntervalsStartingAt(subtree->getLeft(), start);
					}
					else if (compareResult > 0) {
						return GetIntervalsStartingAt(subtree->getRight(), start);
					}
					else {
						auto result = IntervalPairVector();
						if (subtree->getRange().size() > 0) {
							for (auto kvp : subtree->GetRangeReverse()) {
								result->Add(kvp);
							}
						}
						result->Add(new IntervalPair(subtree->getInterval(), subtree->getValue()));
						return result;
					}
				}
				else {
					return nullptr;
				}
			}

			/// <summary>
			/// Searches for all intervals in this subtree that are overlapping the argument interval.
			/// If multiple intervals starting at the same time/value are found to overlap, they are returned in decreasing order of their End values.
			/// </summary>
			/// <param name="toFind">To find.</param>
			/// <param name="list">The list.</param>
			void GetIntervalsOverlappingWith(IntervalType toFind, IntervalPairVector &list) {
				if (toFind.End->compare(this->getInterval().Start) <= 0) {
					////toFind ends before subtree.Data begins, prune the right subtree
					if (this->getLeft() != nullptr) {
						this->getLeft()->GetIntervalsOverlappingWith(toFind, list);
					}
				}
				else if (toFind.Start->compare(this->getMax()) >= 0) {
					////toFind begins after the subtree.Max ends, prune the left subtree
					if (this->getRight() != nullptr) {
						this->getRight()->GetIntervalsOverlappingWith(toFind, list);
					}
				}
				else {
					//// search the left subtree
					if (this->getLeft() != nullptr) {
						this->getLeft()->GetIntervalsOverlappingWith(toFind, list);
					}

					if (this->getInterval().OverlapsWith(toFind)) {
						if (list.empty()) {
							list = IntervalPairVector();
						}

						list.push_back(new IntervalPair(this->getInterval(), this->getValue()));

						////the max value is stored in the node, if the node doesn't overlap then neither are the nodes in its range 
						if (this->getRange().size() > 0 && this->getRange().size() > 0) {
							int rangeCount = this->getRange().size();
							for (auto kvp : this->GetRange()) {
								if (kvp->first->OverlapsWith(toFind)) {
									if (list.empty()) {
										list = IntervalPairVector();
									}
									list.push_back(kvp);
								}
								else {
									break;
								}
							}
						}
					}

					//// search the right subtree
					if (this->getRight() != nullptr) {
						this->getRight()->GetIntervalsOverlappingWith(toFind, list);
					}
				}
			}

			/// <summary>
			/// Gets all intervals in this subtree that are overlapping the argument interval. 
			/// If multiple intervals starting at the same time/value are found to overlap, they are returned in decreasing order of their End values.
			/// </summary>
			/// <param name="toFind">To find.</param>
			/// <returns></returns>
			IEnumerable<IntervalPair*> *GetIntervalsOverlappingWith(IntervalType toFind) {
				if (toFind.End->compare(this->getInterval().Start) <= 0) {
					////toFind ends before subtree.Data begins, prune the right subtree
					if (this->getLeft() != nullptr) {
						for (auto value : this->getLeft()->GetIntervalsOverlappingWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
							yield return value;
						}
					}
				}
				else if (toFind.Start->compare(this->getMax()) >= 0) {
					////toFind begins after the subtree.Max ends, prune the left subtree
					if (this->getRight() != nullptr) {
						for (auto value : this->getRight()->GetIntervalsOverlappingWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
							yield return value;
						}
					}
				}
				else {
					if (this->getLeft() != nullptr) {
						for (auto value : this->getLeft()->GetIntervalsOverlappingWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
							yield return value;
						}
					}

					if (this->getInterval().OverlapsWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
						yield return new IntervalPair(this->getInterval(), this->getValue());

						if (this->getRange().size() > 0 && this->getRange().size() > 0) {
							for (auto kvp : this->GetRange()) {
								if (kvp->first->OverlapsWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
									yield return kvp;
								}
								else {
									break;
								}
							}
						}
					}

					if (this->getRight() != nullptr) {
						for (auto value : this->getRight()->GetIntervalsOverlappingWith(toFind)) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
							yield return value;
						}
					}
				}
			}

			void Visit(VisitNodeHandler<IntervalNode*> *visitor, int level) {
				if (this->getLeft() != nullptr) {
					this->getLeft()->Visit(visitor, level + 1);
				}

				visitor(this, level);

				if (this->getRight() != nullptr) {
					this->getRight()->Visit(visitor, level + 1);
				}
			}

			/// <summary>
			/// Rotates lefts this instance.
			/// Assumes that this.Right != null
			/// </summary>
			/// <returns></returns>
		private:
			static IntervalNode *RotateLeft(IntervalNode *node) {
				auto right = node->getRight();
				assert(node->getRight() != nullptr);

				auto rightLeft = right->getLeft();

				node->setRight(rightLeft);
				ComputeMax(node);

#if defined(TREE_WITH_PARENT_POINTERS)
				auto parent = node->Parent;
				if (rightLeft != nullptr) {
					rightLeft->Parent = node;
				}
#endif
				right->setLeft(node);
				ComputeMax(right);

#if defined(TREE_WITH_PARENT_POINTERS)
				node->Parent = right;
				if (parent != nullptr) {
					if (parent->Left == node) {
						parent->Left = right;
					}
					else {
						parent->Right = right;
					}
				}
				right->Parent = parent;
#endif
				return right;
			}

			/// <summary>
			/// Rotates right this instance.
			/// Assumes that (this.Left != null)
			/// </summary>
			/// <returns></returns>
			static IntervalNode *RotateRight(IntervalNode *node) {
				auto left = node->getLeft();
				assert(node->getLeft() != nullptr);

				auto leftRight = left->getRight();
				node->setLeft(leftRight);
				ComputeMax(node);

#if defined(TREE_WITH_PARENT_POINTERS)
				auto parent = node->Parent;
				if (leftRight != nullptr) {
					leftRight->Parent = node;
				}
#endif
				left->setRight(node);
				ComputeMax(left);

#if defined(TREE_WITH_PARENT_POINTERS)
				node->Parent = left;
				if (parent != nullptr) {
					if (parent->Left == node) {
						parent->Left = left;
					}
					else {
						parent->Right = left;
					}
				}
				left->Parent = parent;
#endif
				return left;
			}

			/// <summary>
			/// Deletes the specified interval from this node. 
			/// If the interval tree is used with unique intervals, this method removes the interval specified as an argument.
			/// If multiple identical intervals (starting at the same time and also ending at the same time) are allowed, this function will delete one of them. 
			/// In this case, it is easy enough to either specify the (interval, value) pair to be deleted or enforce uniqueness by changing the Add procedure.
			/// </summary>
			/// <param name="interval">The interval to be deleted.</param>
			/// <returns></returns>
			bool DeleteIntervalFromNodeWithRange(IntervalType interval) {
				if (this->getRange().size() > 0 && this->getRange().size() > 0) {
					int rangeCount = this->getRange().size();
					int intervalPosition = -1;

					// find the exact interval to delete based on its End value.
					if (interval.End->compare(this->getInterval().End) == 0) {
						intervalPosition = 0;
					}
					else if (rangeCount > 12) {
						auto keyvalueComparer = new PairComparer<T*, TypeValue*>(ComparerUtil::GetComparer());
						int k = this->getRange().BinarySearch(new TypePair(interval.End, default(TypeValue)), keyvalueComparer);
						if (k >= 0) {
							intervalPosition = k + 1;
						}
					}
					else {
						for (int k = 0; k < rangeCount; k++) {
							if (interval.End->compare(this->getRange()[k]->Key) == 0) {
								intervalPosition = k + 1;
								break;
							}
						}
					}

					if (intervalPosition < 0) {
						return false;
					}
					else if (intervalPosition == 0) {
						this->setInterval(IntervalType(this->getInterval().Start, this->getRange()[0]->first));
						this->setValue(this->getRange()[0]->Value);
						this->getRange().RemoveAt(0);
					}
					else if (intervalPosition > 0) {
						this->getRange().RemoveAt(intervalPosition - 1);
					}

					if (this->getRange().empty()) {
						this->getRange().clear();
					}

					return true;
				}
				else {
					////if interval end was not found in the range (or the node itself) or if the node doesnt have a range, return false
					return false;
				}
			}

			void Swap(IntervalNode *node) {
				auto dataInterval = this->getInterval();
				auto dataValue = this->getValue();
				auto dataRange = this->getRange();

				this->setInterval(node->getInterval());
				this->setValue(node->getValue());
				this->setRange(node->getRange());

				node->setInterval(dataInterval);
				node->setValue(dataValue);
				node->setRange(dataRange);
			}

			void AddIntervalValuePair(IntervalType interval, TypeValue *value) {
				if (this->getRange().empty()) {
					this->setRange(TypePairVector());
				}

				////always store the max End value in the node.Data itself .. store the Range list in decreasing order
				if (interval.End->compare(this->getInterval().End) > 0) {
					this->getRange().Insert(0, new TypePair(this->getInterval().End, this->getValue()));
					this->setInterval(interval);
					this->setValue(value);
				}
				else {
					bool wasAdded = false;
					for (int i = 0; i < this->getRange().size(); i++) {
						if (interval.End->compare(this->getRange()[i]->first) >= 0) {
							this->getRange().Insert(i, new TypePair(interval.End, value));
							wasAdded = true;
							break;
						}
					}
					if (!wasAdded) {
						this->getRange().push_back(new TypePair(interval.End, value));
					}
				}
			}

		};

	private:
		template<typename TFirst, typename TSecond>
		class PairComparer : public IComparer<std::pair<TFirst, TSecond>*> {
		private:
			IComparer<TFirst> *keyComparer;

			/// <summary>
			/// Initializes a new instance of the <see cref="IntervalTree&lt;T, TypeValue&gt;.PairComparer&lt;TKey, TSecond&gt;"/> class.
			/// </summary>
			/// <param name="keyComparer">The key comparer.</param>
		public:
			typedef std::pair<TFirst, TSecond> ComparerPair; // was KeyValuePair<TKey, TSecond>
			typedef std::pair<IntervalType*, TypeValue> IntervalTypePair; // was KeyValuePair<IntervalType*, TypeValue>

			PairComparer(IComparer<TFirst> *keyComparer) {
				this->keyComparer = keyComparer;
			}

			/// <summary>
			/// Compares two objects and returns a value indicating whether one is less than, equal to, or greater than the other.
			/// </summary>
			/// <param name="x">The first object to compare.</param>
			/// <param name="y">The second object to compare.</param>
			/// <returns>
			/// Value Condition Less than zero is less than y.Zerox equals y.Greater than zero is greater than y.
			/// </returns>
			int Compare(ComparerPair *x, ComparerPair *y) {
				return (-1) * this->keyComparer->Compare(x->first, y->first);
			}

			/// <summary>
			/// Determines whether the specified <see cref="System.Object"/> is equal to this instance.
			/// </summary>
			/// <param name="obj">The <see cref="System.Object"/> to compare with this instance.</param>
			/// <returns>
			///   <c>true</c> if the specified <see cref="System.Object"/> is equal to this instance; otherwise, <c>false</c>.
			/// </returns>
			virtual bool Equals(void *obj) override {
				if (dynamic_cast<PairComparer<TFirst, TSecond>*>(obj) != nullptr) {
					return Object::Equals(this->keyComparer, (static_cast<PairComparer<TFirst, TSecond>*>(obj))->keyComparer);
				}
				else {
					return false;
				}
			}

			/// <summary>
			/// Returns a hash code for this instance.
			/// </summary>
			/// <returns>
			/// A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table. 
			/// </returns>
			virtual int GetHashCode() override {
				return this->keyComparer->GetHashCode();
			}
		};

	public:
		class ComparerUtil {
		public:
			static IComparer<T*> *GetComparer() {
				if (IComparable<T*>::typeid::IsAssignableFrom(T::typeid) || System::IComparable::typeid::IsAssignableFrom(T::typeid)) {
					return Comparer<T*>::Default;
				}
				else {
					throw InvalidOperationException(std::string::Format("The type {0} cannot be compared. It must implement IComparable<T> or IComparable", T::typeid->FullName));
				}
			}
		};


	private:
		int Count;
		IntervalNode *Root;
		IComparer<T> *comparer;
		PairComparer<T, TypeValue> *keyvalueComparer;



		/// <summary>
		/// Initializes a new instance of the <see cref="IntervalTree&lt;T, TypeValue&gt;"/> class.
		/// </summary>
	public:
		IntervalTree() : IntervalTree(nullptr) {
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="IntervalTree&lt;T, TypeValue&gt;"/> class.
		/// </summary>
		/// <param name="elems">The elems.</param>
		IntervalTree(IEnumerable<IntervalTypePair*> *elems) {
			if (elems != nullptr) {
				for (auto elem : elems) {
					Add(elem->first, elem->second);
				}
			}
			this->comparer = ComparerUtil::GetComparer();
			this->keyvalueComparer = new PairComparer<T, TypeValue>(this->comparer);
		}



		/// <summary>
		/// visitor delegate
		/// </summary>
		/// <typeparam name="TNode">The type of the node.</typeparam>
		/// <param name="node">The node.</param>
		/// <param name="level">The level.</param>
		template<typename TNode>
		typedef void (*VisitNodeHandler)(TNode *node, int level);



		/// <summary>
		/// Adds the specified interval.
		/// If there is more than one interval starting at the same time/value, the intervalnode.Interval stores the start time and the maximum end time of all intervals starting at the same value.
		/// All end values (except the maximum end time/value which is stored in the interval node itself) are stored in the Range list in decreasing order.
		/// Note: this is okay for problems where intervals starting at the same time /value is not a frequent occurrence, however you can use other data structure for better performance depending on your problem needs
		/// </summary>
		/// <param name="arg">The arg.</param>
		void Add(T x, T y, TypeValue value) {
			Add(IntervalType(x, y), value);
		}

		/// <summary>
		/// Adds the specified interval.
		/// If there is more than one interval starting at the same time/value, the intervalnode.Interval stores the start time and the maximum end time of all intervals starting at the same value.
		/// All end values (except the maximum end time/value which is stored in the interval node itself) are stored in the Range list in decreasing order.
		/// Note: this is okay for problems where intervals starting at the same time /value is not a frequent occurrence, however you can use other data structure for better performance depending on your problem needs
		/// </summary>
		/// <param name="arg">The arg.</param>
		bool Add(IntervalType interval, TypeValue value) {
			bool wasAdded = false;
			bool wasSuccessful = false;

			this->Root = IntervalNode::Add(this->Root, interval, value, wasAdded, wasSuccessful);
			if (this->Root != nullptr) {
				IntervalNode::ComputeMax(this->Root);
			}

			if (wasSuccessful) {
				this->Count++;
			}

			return wasSuccessful;
		}

		/// <summary>
		/// Deletes the specified interval.
		/// If the interval tree is used with unique intervals, this method removes the interval specified as an argument.
		/// If multiple identical intervals (starting at the same time and also ending at the same time) are allowed, this function will delete one of them( see procedure DeleteIntervalFromNodeWithRange for details)
		/// In this case, it is easy enough to either specify the (interval, value) pair to be deleted or enforce uniqueness by changing the Add procedure.
		/// </summary>
		/// <param name="arg">The arg.</param>
		bool Delete(IntervalType arg) {
			if (this->Root != nullptr) {
				bool wasDeleted = false;
				bool wasSuccessful = false;

				this->Root = IntervalNode::Delete(this->Root, arg, wasDeleted, wasSuccessful);
				if (this->Root != nullptr) {
					IntervalNode::ComputeMax(this->Root);
				}

				if (wasSuccessful) {
					this->Count--;
				}
				return wasSuccessful;
			}
			else {
				return false;
			}
		}

		/// <summary>
		/// Searches for all intervals overlapping the one specified.
		/// If multiple intervals starting at the same time/value are found to overlap the specified interval, they are returned in decreasing order of their End values.
		/// </summary>
		/// <param name="toFind">To find.</param>
		/// <param name="list">The list.</param>
		void GetIntervalsOverlappingWith(IntervalType toFind, std::vector<IntervalTypePair*> &list) {
			if (this->Root != nullptr) {
				this->Root->GetIntervalsOverlappingWith(toFind, list);
			}
		}

		/// <summary>
		/// Searches for all intervals overlapping the one specified.
		/// If multiple intervals starting at the same time/value are found to overlap the specified interval, they are returned in decreasing order of their End values.
		/// </summary>
		/// <param name="toFind">To find.</param>
		/// <returns></returns>
		IEnumerable<IntervalTypePair*> *GetIntervalsOverlappingWith(IntervalType toFind) {
			return (this->Root != nullptr) ? this->Root->GetIntervalsOverlappingWith(toFind) : nullptr;
		}

		/// <summary>
		/// Returns all intervals beginning at the specified start value. 
		/// The multiple intervals start at the specified value, they are sorted based on their End value (i.e. returned in ascending order of their End values)
		/// </summary>
		/// <param name="arg">The arg.</param>
		/// <returns></returns>
		std::vector<IntervalTypePair*> GetIntervalsStartingAt(T arg) {
			return IntervalNode::GetIntervalsStartingAt(this->Root, arg);
		}

#if defined(TREE_WITH_PARENT_POINTERS)

		/// <summary>
		/// Gets the collection of intervals (in ascending order of their Start values).
		/// Those intervals starting at the same time/value are sorted further based on their End value (i.e. returned in ascending order of their End values)
		/// </summary>
		const IEnumerable<IntervalType*> &getIntervals() const {
			if (this->Root == nullptr) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield break;
			}

			auto p = IntervalNode::FindMin(this->Root);
			while (p != nullptr) {
				for (auto rangeNode : p->GetRangeReverse()) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
					yield return rangeNode->first;
				}

//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield return p->getInterval();
				p = p->Successor();
			}
		}

		/// <summary>
		/// Gets the collection of values (ascending order)
		/// Those intervals starting at the same time/value are sorted further based on their End value (i.e. returned in ascending order of their End values)
		/// </summary>
		const IEnumerable<TypeValue> &getValues() const {
			if (this->Root == nullptr) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield break;
			}

			auto p = IntervalNode::FindMin(this->Root);
			while (p != nullptr) {
				for (auto rangeNode : p->GetRangeReverse()) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
					yield return rangeNode->Value;
				}

//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield return p->getValue();
				p = p->Successor();
			}
		}

		/// <summary>
		/// Gets the interval value pairs.
		/// Those intervals starting at the same time/value are sorted further based on their End value (i.e. returned in ascending order of their End values)
		/// </summary>
		const IEnumerable<IntervalTypePair*> &getIntervalValuePairs() const {
			if (this->Root == nullptr) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield break;
			}

			auto p = IntervalNode::FindMin(this->Root);
			while (p != nullptr) {
				for (auto rangeNode : p->GetRangeReverse()) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
					yield return rangeNode;
				}

//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
				yield return new IntervalTypePair(p->getInterval(), p->getValue());
				p = p->Successor();
			}
		}

#endif

		/// <summary>
		/// Tries to the get the value associated with the interval.
		/// </summary>
		/// <param name="subtree">The subtree.</param>
		/// <param name="data">The data.</param>
		/// <param name="value">The value.</param>
		/// <returns></returns>
		bool TryGetInterval(IntervalType data, TypeValue &value) {
			return this->TryGetIntervalImpl(this->Root, data, value);
		}

		/// <summary>
		/// Clears this instance.
		/// </summary>
		void Clear() {
//C# TO C++ CONVERTER WARNING: C# to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
			delete this->Root;
			this->Count = 0;
		}

#if 0
		/// <summary>
		/// Prints this instance (to console).
		/// </summary>
		void Print() {
			this->Visit([&] (node, level) {
				std::cout << std::string(' ', 2 * level);
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
				std::cout << std::string::Format("{0}.{1}", node::Interval->ToString(), node::Max);
				if (node::Range != nullptr) {
					std::cout << " ... ";
					for (auto rangeNode : node::GetRange()) {
						std::cout << std::string::Format("{0}  ", rangeNode->first);
					}
				}
				std::cout << std::endl;
			});
		}
#endif

		/// <summary>
		/// Searches for interval starting at.
		/// </summary>
		/// <param name="subtree">The subtree.</param>
		/// <param name="data">The data.</param>
		/// <returns></returns>
	private:
		bool TryGetIntervalImpl(IntervalNode *subtree, IntervalType data, TypeValue &value) {
			if (subtree != nullptr) {
				int compareResult = data.Start->compare(subtree->getInterval().Start);

				if (compareResult < 0) {
					return this->TryGetIntervalImpl(subtree->getLeft(), data, value);
				}
				else if (compareResult > 0) {
					return this->TryGetIntervalImpl(subtree->getRight(), data, value);
				}
				else {
					if (data.End->compare(subtree->getInterval().End) == 0) {
						value = subtree->getValue();
						return true;
					}
					else if (subtree->getRange().size() > 0) {
						int kthIndex = subtree->getRange().BinarySearch(new KeyValuePair<T, TypeValue>(data.End, TypeValue()), this->keyvalueComparer);
						if (kthIndex >= 0) {
							value = subtree->getRange()[kthIndex]->Value;
							return true;
						}
					}
				}
			}
			value = TypeValue();
			return false;
		}

		/// <summary>
		/// Visit_inorders the specified visitor. Defined for debugging purposes only
		/// </summary>
		/// <param name="visitor">The visitor.</param>
		void Visit(VisitNodeHandler<IntervalNode*> visitor) {
			if (this->Root != nullptr) {
				this->Root->Visit(visitor, 0);
			}
		}



		/// <summary>
		/// IntervalNode class. 
		/// </summary>
		/// <typeparam name="TElem">The type of the elem.</typeparam>
	};

}


#endif	//#ifndef INTERVALTREE
