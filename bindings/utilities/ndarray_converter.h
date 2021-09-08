//
// Created by per on 9/8/21.
//

# ifndef __NDARRAY_CONVERTER_H__
# define __NDARRAY_CONVERTER_H__

#include <Python.h>
#include <opencv2/core/core.hpp>
#include <pybind11/pybind11.h>


class NDArrayConverter {
public:
    // must call this first, or the other routines don't work!
    static bool init_numpy();

    static bool toMat(PyObject* o, cv::Mat &m);
    static PyObject* toNDArray(const cv::Mat& mat);
};

namespace pybind11::detail {

    template <> struct type_caster<cv::Mat>: public type_caster_base<cv::Mat> {
        using base = type_caster_base<cv::Mat>;
    public:

        bool load(handle src, bool /* convert */) {
            return NDArrayConverter::toMat(src.ptr(), *this);
        }

        static handle cast(const cv::Mat &m, return_value_policy, handle defval) {
            return handle(NDArrayConverter::toNDArray(m));
        }
    };


} // namespace pybind11::detail

# endif

