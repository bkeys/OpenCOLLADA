#include "XmlNodeSet.h"
#include "XmlNode.h"

using namespace std;

namespace opencollada
{
	XmlNodeSet XmlNodeSet::null;

	XmlNodeSet::XmlNodeSet(XmlNodeSet&& other)
	{
		*this = std::move(other);
	}

	XmlNodeSet::~XmlNodeSet()
	{
		if (mXPathObject)
		{
			xmlXPathFreeObject(mXPathObject);
			mXPathObject = nullptr;
		}
	}

	XmlNodeSet::XmlNodeSet(xmlXPathObjectPtr xpathObject)
		: mXPathObject(xpathObject)
	{}

	const XmlNodeSet & XmlNodeSet::operator = (XmlNodeSet && other)
	{
		swap(mXPathObject, other.mXPathObject);
		return *this;
	}

	XmlNode XmlNodeSet::iterator::operator*() const
	{
		return XmlNode(xmlXPathNodeSetItem(mNodeSet, mIndex));
	}

	void XmlNodeSet::iterator::operator++()
	{
		++mIndex;
		if (mIndex == mNodeSet->nodeNr)
		{
			mNodeSet = nullptr;
			mIndex = 0;
		}
	}

	bool XmlNodeSet::iterator::operator != (const iterator & other) const
	{
		return mNodeSet != other.mNodeSet || mIndex != other.mIndex;
	}

	XmlNodeSet::iterator::iterator(xmlNodeSetPtr nodeSet)
		: mNodeSet(nodeSet)
	{}

	bool XmlNodeSet::empty() const
	{
		return size() == 0;
	}

	XmlNodeSet::iterator XmlNodeSet::begin() const
	{
		if (!mXPathObject)
			return iterator();
		if (!mXPathObject->nodesetval)
			return iterator();
		if (mXPathObject->nodesetval->nodeNr == 0)
			return iterator();
		return iterator(mXPathObject->nodesetval);
	}

	XmlNodeSet::iterator XmlNodeSet::end() const
	{
		return iterator();
	}

	size_t XmlNodeSet::size() const
	{
		if (!mXPathObject)
			return 0;
		return static_cast<size_t>(xmlXPathNodeSetGetLength(mXPathObject->nodesetval));
	}

	XmlNode XmlNodeSet::operator[](int index) const
	{
		return XmlNode(xmlXPathNodeSetItem(mXPathObject->nodesetval, index));
	}
}
