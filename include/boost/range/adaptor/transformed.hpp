// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_ADAPTOR_TRANSFORMED_HPP
#define BOOST_RANGE_ADAPTOR_TRANSFORMED_HPP

#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/concepts.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/utility/result_of.hpp>

namespace boost
{
    namespace range_detail
    {

        template< class F, class R >
        struct transformed_range :
            public boost::iterator_range<
                      boost::transform_iterator< F,
                          BOOST_DEDUCED_TYPENAME range_iterator<R>::type
                                              >
                                         >
        {
        private:
            typedef boost::iterator_range<
                      boost::transform_iterator< F,
                        BOOST_DEDUCED_TYPENAME range_iterator<R>::type
                                              >
                                         >
                base;

        public:
            typedef F transform_fn_type;
            typedef R source_range_type;

            transformed_range( F f, R& r )
                : base( boost::make_transform_iterator( boost::begin(r), f ),
                        boost::make_transform_iterator( boost::end(r), f ) )

            {
            }
        };

        template< class T >
        struct transform_holder : holder<T>
        {
            transform_holder( T r ) : holder<T>(r)
            {
            }
        };

        template< class SinglePassRange, class UnaryFunction >
        inline transformed_range<UnaryFunction,SinglePassRange>
        operator|( SinglePassRange& r,
                   const transform_holder<UnaryFunction>& f )
        {
            BOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<SinglePassRange>));

            return transformed_range<UnaryFunction,SinglePassRange>( f.val, r );
        }

        template< class SinglePassRange, class UnaryFunction >
        inline transformed_range<UnaryFunction, const SinglePassRange>
        operator|( const SinglePassRange& r,
                   const transform_holder<UnaryFunction>& f )
        {
            BOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<const SinglePassRange>));

           return transformed_range<UnaryFunction, const SinglePassRange>(
               f.val, r);
        }

    } // 'range_detail'

    using range_detail::transformed_range;

    namespace adaptors
    {
        namespace
        {
            const range_detail::forwarder<range_detail::transform_holder>
                    transformed =
                      range_detail::forwarder<range_detail::transform_holder>();
        }

        template<class UnaryFunction, class SinglePassRange>
        inline transformed_range<UnaryFunction, SinglePassRange>
        transform(SinglePassRange& rng, UnaryFunction fn)
        {
            BOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<SinglePassRange>));

            return transformed_range<UnaryFunction, SinglePassRange>(fn, rng);
        }

        template<class UnaryFunction, class SinglePassRange>
        inline transformed_range<UnaryFunction, const SinglePassRange>
        transform(const SinglePassRange& rng, UnaryFunction fn)
        {
            BOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<const SinglePassRange>));

            return transformed_range<UnaryFunction, const SinglePassRange>(
                fn, rng);
        }
    } // 'adaptors'

}

#endif
