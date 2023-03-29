#pragma once

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // ������ ������ �������� ��������.
    // ���������� ��������� ��������� �� �������� ������������ ������
    // ValueType � ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        // ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

        // ��������� ��������� � forward iterator
        // (��������, ������� ������������ �������� ���������� � ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� ����� �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto value_old(*this);
            node_ = node_->next_node;
            return value_old;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }
    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() {}

    SingleLinkedList(std::initializer_list<Type> values) {
        Init(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Init(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs)
        {
            SingleLinkedList buffer(rhs);
            swap(buffer);
        }
        return *this;
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    // ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

    // ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    /*
     * ��������� ������� value ����� ��������, �� ������� ��������� pos.
     * ���������� �������� �� ����������� �������
     * ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        size_++;
        return Iterator{ pos.node_->next_node };
    }

    /*
     * ������� �������, ��������� �� pos.
     * ���������� �������� �� �������, ��������� �� ��������
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        Node* buffer = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = buffer;
        size_--;
        return Iterator{ pos.node_->next_node };
    }

    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    // ��������� ������� value � ������ ������ �� ����� O(1)
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        size_++;
    }

    // ������� ������ �� ����� O(N)
    void Clear() noexcept {
        while (head_.next_node)
        {
            Node* head_new = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = head_new;
        }
        size_ = 0;
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    void PopFront() noexcept {
        assert(!IsEmpty());
        Node* buffer = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = buffer;
        size_--;
    }

    ~SingleLinkedList()
    {
        Clear();
    }

private:
    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_ = 0;

    template <typename It>
    void Init(It begin, It end)
    {
        SingleLinkedList buffer;
        Node* node = &buffer.head_;
        for (It i = begin; i != end; i++)
        {
            buffer.size_++;
            node->next_node = new Node(*i, nullptr);
            node = node->next_node;
        }
        swap(buffer);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() == rhs.GetSize())
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    return false;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs <= lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}