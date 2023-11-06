void RedBlackTree::leftRotate(Node* node)
{
	if (_nil == node || _nil == node->_right)
		return;

	Node* temp = node->_right;
	node->_right = node->_right->_left;
	if(_nil != node->_right)
		node->_right->_parent = node;
	temp->_left = node;
	temp->_parent = node->_parent;
	if (nullptr != node->_parent)
	{
		if (node->_parent->_left == node)
			node->_parent->_left = temp;
		else
			node->_parent->_right = temp;
	}
	node->_parent = temp;
	if (_root == node)
		_root = temp;
}