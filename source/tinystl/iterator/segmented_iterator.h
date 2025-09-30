#pragma once

namespace tinystl {

template <class Iter>
struct segmented_iterator_traits;
/*
{
using segment_iterator = ...;
using local_iterator   = ...;

static segment_iterator segment(Iter);
static local_iterator local(Iter);
static local_iterator begin(segment_iterator);
static local_iterator end(segment_iterator);
static Iter compose(segment_iterator, local_iterator);
};
*/

} // namespace tinystl
