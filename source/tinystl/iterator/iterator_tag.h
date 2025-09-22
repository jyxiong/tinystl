#pragma once

// https://en.cppreference.com/w/cpp/iterator/iterator_tags.html
// https://en.cppreference.com/w/cpp/iterator/iterator_traits.html

namespace tinystl {
// iteratror category tags
struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : public input_iterator_tag {};

struct bidirectional_iterator_tag : public forward_iterator_tag {};

struct random_access_iterator_tag : public bidirectional_iterator_tag {};

struct contiguous_iterator_tag : public random_access_iterator_tag {};


} // namespace tinystl
