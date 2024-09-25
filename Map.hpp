#ifndef MAP_H
#define MAP_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <random>
#include <functional>

namespace cs440
{
    //  Map Class
    template <typename Key_T, typename Mapped_T>
    class Map
    {
    public:
        using ValueType = std::pair<const Key_T, Mapped_T>;
        //  Subclasses

        class Node
        {
        public:
            std::vector<Node *> forward;
            Node *backward;
            bool isNull;
            Node(int level, bool n = true) : forward(level + 1, nullptr), backward(nullptr), isNull(n) {}
        };

        class NodeWithValue : public Node
        {
        public:
            ValueType val;
            NodeWithValue(const ValueType v, int level) : Node(level, false), val(v) {}
        };

        //  Iterator Class
        class Iterator
        {
        public:
            typename Map::Node *node;
            Iterator(Node *newNode) : node(newNode) {}
            Iterator(const Iterator &newNode) : node(newNode.node) {}
            Iterator &operator++()
            {
                Iterator &temp = *this;
                node = node->forward[0];
                return temp;
            }
            Iterator operator++(int)
            {
                node = node->forward[0];
                return *this;
            }
            Iterator &operator--()
            {
                Iterator &temp = *this;
                node = node->backward;
                return temp;
            }
            Iterator operator--(int)
            {
                node = node->backward;
                return *this;
            }
            ValueType &operator*() const
            {
                return static_cast<NodeWithValue *>(node)->val;
            }
            ValueType *operator->() const
            {
                return &static_cast<NodeWithValue *>(node)->val;
            }
            bool operator==(const Iterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return true;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return false;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            bool operator!=(const Iterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return false;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return true;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            Node *getNode()
            {
                return node;
            }
        };

        //  ReverseIterator Class
        class ReverseIterator
        {
        public:
            ReverseIterator(Node *newNode) : node(newNode) {}
            ReverseIterator &operator++()
            {
                ReverseIterator &temp = *this;
                node = node->backward;
                return temp;
            }
            ReverseIterator operator++(int)
            {
                node = node->backward;
                return *this;
            }
            ReverseIterator &operator--()
            {
                ReverseIterator &temp = *this;
                node = node->forward[0];
                return temp;
            }
            ReverseIterator operator--(int)
            {
                node = node->forward[0];
                return *this;
            }
            ValueType &operator*() const
            {
                return static_cast<NodeWithValue *>(node)->val;
            }
            ValueType *operator->() const
            {
                return &static_cast<NodeWithValue *>(node)->val;
            }
            bool operator==(const ReverseIterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return true;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return false;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            bool operator!=(const ReverseIterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return false;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return true;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            Node *getNode()
            {
                return node;
            }

        private:
            Node *node;
        };

        //  ConstIterator Class
        class ConstIterator
        {
        public:
            ConstIterator(Node *newNode) : node(newNode) {}
            ConstIterator(const Iterator &newNode) : node(newNode.node) {}
            ConstIterator &operator++()
            {
                ConstIterator &temp = *this;
                node = node->forward[0];
                return temp;
            }
            ConstIterator operator++(int)
            {
                node = node->forward[0];
                return *this;
            }
            ConstIterator &operator--()
            {
                ConstIterator &temp = *this;
                node = node->backward;
                return temp;
            }
            ConstIterator operator--(int)
            {
                node = node->backward;
                return *this;
            }
            const ValueType &operator*() const
            {
                return static_cast<NodeWithValue *>(node)->val;
            }
            const ValueType *operator->() const
            {
                return &static_cast<NodeWithValue *>(node)->val;
            }
            bool operator==(const ConstIterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return true;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return false;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            bool operator!=(const ConstIterator &newNode)
            {
                if (node->isNull && newNode.node->isNull) {
                    return false;
                }
                else if (!node->isNull || newNode.node->isNull) {
                    return true;
                }
                else
                {
                    return (static_cast<NodeWithValue *>(node)->val == static_cast<NodeWithValue *>(newNode.node)->val);
                }
            }
            Node *getNode()
            {
                return node;
            }

        private:
            Node *node;
        };

        //  Constructor and Assignment
        Map()
        {
            maxLevel = 20;
            numLevels = 0;
            numNodes = 0;
            head = new Node(maxLevel);
            tail = new Node(maxLevel);
            for (int i = 0; i < maxLevel; ++i)
            {
                head->forward[i] = tail;
            }
            tail->backward = head;
            head->backward = nullptr;
        }
        Map(const Map &oldMap)
        {
            maxLevel = oldMap.getMaxLevel();
            numLevels = 0;
            head = new Node(maxLevel);
            tail = new Node(maxLevel);
            for (int i = 0; i < maxLevel; ++i)
            {
                head->forward[i] = tail;
            }
            tail->backward = head;
            for (const auto &node : oldMap)
            {
                insert({node.first, node.second});
            }
        }
        Map &operator=(const Map &oldMap)
        {
            if (this == &oldMap)
            {
                return *this;
            }
            clear();
            for (const auto &node : oldMap)
            {
                insert({node.first, node.second});
            }
            return *this;
        }
        Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list)
        {
            maxLevel = 20;
            numLevels = 0;
            numNodes = 0;
            head = new Node(maxLevel);
            tail = new Node(maxLevel);
            for (int i = 0; i < maxLevel; i++)
            {
                head->forward[i] = tail;
            }
            tail->backward = head;
            for (const auto &node : list)
            {
                insert({node.first, node.second});
            }
        }
        ~Map()
        {
            Node *cur = head;
            while (cur != tail)
            {
                Node *next = cur->forward[0];
                delete cur;
                cur = next;
            }
        }
        Node *getHead()
        {
            return head;
        }

        //  Size
        size_t size() const
        {
            return (size_t)numNodes;
        }
        bool empty() const
        {
            return numNodes == 0;
        }

        //  Iterators
        Iterator begin()
        {
            return Iterator(head->forward[0]);
        }
        Iterator end()
        {
            return Iterator(tail);
        }
        ConstIterator begin() const
        {
            return ConstIterator(head->forward[0]);
        }
        ConstIterator end() const
        {
            return ConstIterator(tail);
        }
        ReverseIterator rbegin() const
        {
            return ReverseIterator(tail->backward);
        }
        ReverseIterator rend() const
        {
            return ReverseIterator(head);
        }

        //  Element access
        Iterator find(const Key_T &key)
        {
            Node *cur = head;
            for (int i = numLevels; i >= 0; i--)
            {
                while (cur->forward[i] != tail && static_cast<NodeWithValue *>(cur->forward[i])->val.first < key)
                {
                    cur = cur->forward[i];
                }
            }
            cur = cur->forward[0];
            if (cur != tail && static_cast<NodeWithValue *>(cur)->val.first == key)
            {
                return Iterator(cur);
            }
            return end();
        }
        ConstIterator find(const Key_T &key) const
        {
            Node *cur = head;
            for (int i = numLevels; i >= 0; i--)
            {
                while (cur->forward[i] != tail && static_cast<NodeWithValue *>(cur->forward[i])->val.first < key)
                {
                    cur = cur->forward[i];
                }
            }
            cur = cur->forward[0];
            if (cur != tail && static_cast<NodeWithValue *>(cur)->val.first == key)
            {
                return ConstIterator(cur);
            }
            return end();
        }
        Mapped_T &at(const Key_T &key)
        {
            auto it = find(key);
            if (it != end() && static_cast<NodeWithValue *>(it.getNode())->val.first == key)
            {
                return static_cast<NodeWithValue *>(it.getNode())->val.second;
            }
            else
            {
                throw std::out_of_range("not found (in at)");
            }
        }
        const Mapped_T &at(const Key_T &key) const
        {
            ConstIterator it = find(key);
            if (it != end() && static_cast<NodeWithValue *>(it.getNode())->val.first == key)
            {
                return static_cast<NodeWithValue *>(it.getNode())->val.second;
            }
            else
            {
                throw std::out_of_range("not found (in at)");
            }
        }
        Mapped_T &operator[](const Key_T &key)
        {
            auto it = find(key);
            if (it == end())
            {
                insert({key, Mapped_T()});
                it = find(key);
            }
            return static_cast<NodeWithValue *>(it.getNode())->val.second;
        }

        //  Modifiers
        std::pair<Iterator, bool> insert(const ValueType &pair)
        {
            std::vector<Node *> nodesToUpdate(maxLevel + 1, nullptr);
            Node *cur = head;
            //  start at highest level (fast lane)
            for (int i = numLevels; i >= 0; i--)
            {
                //  traverse current lane until you hit a key thats bigger
                while (cur->forward[i] != tail && static_cast<NodeWithValue *>(cur->forward[i])->val.first < pair.first)
                {
                    cur = cur->forward[i];
                }
                //  jot down whatever node we hit so we can update its next ptr
                nodesToUpdate[i] = cur;
                //  so now in next loop iteration, we're going down one from where we landed
                //  so traversal looks smtn like this
                //  - - -
                //      |
                //      - -
                //        |
                //        - -
            }
            cur = cur->forward[0];
            if (cur == tail || !(static_cast<NodeWithValue *>(cur)->val.first == pair.first))
            {
                int level = getRandomLevel(maxLevel);
                if (level > numLevels)
                {
                    for (int i = numLevels + 1; i < level + 1; i++)
                    {
                        nodesToUpdate[i] = head;
                    }
                    numLevels = level;
                }

                Node *newNode = new NodeWithValue(pair, level);
                for (int i = 0; i <= level; i++)
                {
                    newNode->forward[i] = nodesToUpdate[i]->forward[i];
                    nodesToUpdate[i]->forward[i] = newNode;
                }

                //  update backward ptrs in bottom level
                if (newNode->forward[0])
                {
                    newNode->forward[0]->backward = newNode;
                }
                newNode->backward = nodesToUpdate[0];
                numNodes++;
                // std::cout << "Successfully inserted node " << pair.first << std::endl;
                return {Iterator(newNode), true};
            }
            else
            {

                // std::cout << "Tried inserting " << pair.first << ", already exists" << std::endl;
                return {Iterator(cur), false};
            }
        }
        template <typename IT_T>
        void insert(IT_T range_beg, IT_T range_end)
        {
            for (IT_T it = range_beg; it != range_end; it++)
            {
                insert(*it);
            }
        }
        void erase(Iterator pos)
        {
            if (pos == tail)
            {
                return;
            }

            std::vector<Node *> nodesToUpdate(maxLevel, nullptr);
            Node *cur = head;
            Node *target = pos.getNode();

            //  start at highest level (fast lane)
            for (int i = numLevels; i >= 0; i--)
            {
                //  traverse current lane until you hit a key thats bigger
                while (cur->forward[i] && static_cast<NodeWithValue *>(cur->forward[i])->val.first < static_cast<NodeWithValue *>(target)->val.first)
                {
                    cur = cur->forward[i];
                }
                //  jot down whatever node we hit so we can update its next ptr
                nodesToUpdate[i] = cur;
            }
            cur = cur->forward[0];

            if (cur != target)
            {
                return;
            }

            for (int i = 0; i <= numLevels; i++)
            {
                if (nodesToUpdate[i]->forward[i] != cur)
                {
                    break;
                }
                nodesToUpdate[i]->forward[i] = cur->forward[i];
            }

            //  update backwards
            if (cur->forward[0])
            {
                cur->forward[0]->backward = cur->backward;
            }

            delete cur;
            numNodes--;
            while (numLevels > 0 && head->forward[numLevels] == tail)
            {
                numLevels--;
            }
        }
        void erase(const Key_T &key)
        {
            std::vector<Node *> nodesToUpdate(maxLevel, nullptr);
            Node *cur = head;

            //  start at highest level (fast lane)
            for (int i = numLevels; i >= 0; i--)
            {
                //  traverse current lane until you hit a key thats bigger
                while (cur->forward[i] != tail && static_cast<NodeWithValue *>(cur->forward[i])->val.first < key)
                {
                    cur = cur->forward[i];
                }
                //  jot down whatever node we hit so we can update its next ptr
                nodesToUpdate[i] = cur;
            }
            cur = cur->forward[0];
            //  if key exists
            if (cur != tail)
            {
                for (int i = 0; i <= numLevels; i++)
                {
                    if (nodesToUpdate[i]->forward[i] != cur)
                    {
                        break;
                    }
                    nodesToUpdate[i]->forward[i] = cur->forward[i];
                }
                delete cur;
                numNodes--;
                while (numLevels > 0 && head->forward[numLevels] == tail)
                {
                    numLevels--;
                }
            }
        }
        void clear()
        {
            Node *cur = head->forward[0];

            while (cur != tail)
            {
                Node *temp = cur;
                cur = cur->forward[0];
                delete temp;
            }

            numLevels = 0;
            numNodes = 0;
            for (int i = 0; i < maxLevel; ++i)
            {
                head->forward[i] = tail;
            }
        }

        //  Comparison
        bool operator==(const Map &map2)
        {
            if (size() != map2.size())
            {
                return false;
            }

            Iterator map1It = begin();
            Iterator map2It = map2.begin();
            Iterator *map1End = end();
            Iterator *map2End = map2.end();

            while (map1It != map1End && map2It != map2End)
            {
                if (*map1It != *map2It)
                {
                    return false;
                }
                map1It++;
                map2It++;
            }

            return true;
        }
        bool operator!=(const Map &map2)
        {
            if (size() != map2.size())
            {
                return true;
            }

            Iterator map1It = begin();
            Iterator map2It = map2.begin();
            Iterator *map1End = end();
            Iterator *map2End = map2.end();

            while (map1It != map1End && map2It != map2End)
            {
                if (*map1It != *map2It)
                {
                    return true;
                }
                map1It++;
                map2It++;
            }

            return false;
        }
        bool operator<(const Map &map2)
        {
            Iterator map1It = begin();
            Iterator map2It = map2.begin();
            Iterator *map1End = end();
            Iterator *map2End = map2.end();

            while (map1It != map1End && map2It != map2End)
            {
                ValueType element1 = *map1It.getNode()->val;
                ValueType element2 = *map2It.getNode()->val;
                if (element1 < element2)
                {
                    return true;
                }
                map1It++;
                map2It++;
            }

            if (map2It != map2End)
            {
                return true;
            }

            return false;
        }

        static int getRandomLevel(int maxLevel)
        {
            /*
            int curLevel = 0;
            static std::default_random_engine generator(std::random_device{}());
            static std::bernoulli_distribution distribution(0.5);
            while (curLevel < maxLevel - 1 && distribution(generator))
            {
                curLevel++;
            }
            return curLevel;
            */
           /*
            float r = (float)rand() / RAND_MAX;
            int lvl = 0;
            while (r < .5 && lvl < maxLevel)
            {
                lvl++;
                r = (float)rand() / RAND_MAX;
            }
            return lvl;
            */
            //  try drand48()
            
            int curLevel = 0;
            while ((curLevel < maxLevel - 1) && (drand48() > .5)) {
                curLevel++;
            }
            return curLevel;
            
        }

        int getMaxLevel() const
        {
            return maxLevel;
        }

        void printMap()
        {
            for (int i = 0; i <= numLevels; i++)
            {
                Node *cur = head->forward[i];
                std::cout << "Level " << i << ": ";
                while (cur != tail)
                {
                    std::cout << "{" << static_cast<NodeWithValue *>(cur)->val.first << ", " << static_cast<NodeWithValue *>(cur)->val.second << "} ";
                    cur = cur->forward[0];
                }
                std::cout << std::endl << std::endl << std::endl;
            }
        }

        //      Comparison Operators        //
        friend bool operator==(const Iterator &it1, const ConstIterator &it2)
        {
            return (it1.getNode()->val == it2.getNode()->val);
        }
        friend bool operator==(const ConstIterator &it1, const Iterator &it2)
        {
            return (it1.getNode()->val == it2.getNode()->val);
        }
        friend bool operator!=(const Iterator &it1, const ConstIterator &it2)
        {
            return (it1.getNode()->val != it2.getNode()->val);
        }
        friend bool operator!=(const ConstIterator &it1, const Iterator &it2)
        {
            return (it1.getNode()->val != it2.getNode()->val);
        }

    private:
        Node *head;
        Node *tail;
        int numNodes = 0;
        int maxLevel = 50;
        int numLevels = 1;
    }; //  end class Map

} // end namespace

#endif