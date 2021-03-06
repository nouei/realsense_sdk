// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

#pragma once
#include "stdint.h"

/**
 * @file types.h
 * @brief common types definitions.
 */

namespace rs
{
    namespace core
    {
        /**
         * @brief construct a unique id from a given set of characters.
         * @param X1        char 1
         * @param X2        char 2
         * @param X3        char 3
         * @param X4        char 4
         * @return int32_t  the unique id.
         */
        inline int32_t CONSTRUCT_UID(char X1,char X2,char X3,char X4)
        {
            return (((unsigned int)(X4)<<24)+((unsigned int)(X3)<<16)+((unsigned int)(X2)<<8)+(unsigned int)(X1));
        }

        /**
        * @enum rotation
        * @brief Image rotation options.
        */
        enum class rotation
        {
            rotation_0_degree   = 0x0,   /**< 0 Degree rotation             */
            rotation_90_degree  = 90,    /**< 90 degree clockwise rotation  */
            rotation_180_degree = 180,   /**< 180 degree clockwise rotation */
            rotation_270_degree = 270,   /**< 270 degree clockwise rotation */
            rotation_invalid_value = -1
        };

        /**
         * @struct device_info
         * @brief device_info decribes the device details.
         */
        struct device_info
        {
            char                name[224];    /**< device name                                 */
            char                serial[32];   /**< serial number                               */
            char                firmware[32]; /**< firmware version                            */
            rs::core::rotation  rotation;     /**< how the camera device is physically mounted */
        };

        /**
         * @struct sizeI32
         */
        struct sizeI32
        {
            int32_t width, height;
        };

        /**
         * @enum sample_flags
         */
        enum class sample_flags
        {
            none,     /**< no special flags */
            external  /**< sample generated from external device (platform camera \ external imu) */
        };

        /**
         * @enum stream_type
         */
        enum class stream_type : int32_t
        {
            depth                            = 0,  /**< Native stream of depth data produced by the device                                                   */
            color                            = 1,  /**< Native stream of color data captured by the device                                                   */
            infrared                         = 2,  /**< Native stream of infrared data captured by the device                                                */
            infrared2                        = 3,  /**< Native stream of infrared data captured from a second viewpoint by the device                        */
            fisheye                          = 4,

            rectified_color                  = 6,  /**< Synthetic stream containing undistorted color data with no extrinsic rotation from the depth stream  */
            max                                    /**< Max number of stream types, must be last                                                             */
        };

        /**
         * @enum pixel_format
         */
        enum class pixel_format : int32_t
        {
            any         = 0,
            z16         = 1,  /**< 16 bit linear depth values. The depth is meters is equal to depth scale * pixel value     */
            disparity16 = 2,  /**< 16 bit linear disparity values. The depth in meters is equal to depth scale / pixel value */
            xyz32f      = 3,  /**< 32 bit floating point 3D coordinates.                                                     */
            yuyv        = 4,
            rgb8        = 5,
            bgr8        = 6,
            rgba8       = 7,
            bgra8       = 8,
            y8          = 9,
            y16         = 10,
            raw8        = 11,
            raw10       = 12, /**< Four 10-bit luminance values encoded into a 5-byte macropixel                             */
            raw16       = 13
        };

        /**
         * @struct distortion_type
         */
        enum class distortion_type : int32_t
        {
            none                   = 0, /**< Rectilinear images, no distortion compensation required                                                             */
            modified_brown_conrady = 1, /**< Equivalent to Brown-Conrady distortion, except that tangential distortion is applied to radially distorted points   */
            inverse_brown_conrady  = 2, /**< Equivalent to Brown-Conrady distortion, except undistorts image instead of distorting it                            */
            distortion_ftheta      = 3
        };

        /**
         * @struct motion_device_intrinsics
         * @brief motion_device_intrinsics represents the motion sensor scale, bias and variances.
         */
        struct motion_device_intrinsics
        {
            float data[3][4];           /**< Scale_X        cross_axis        cross_axis      Bias_X  <br>
                                             cross_axis     Scale_Y           cross_axis      Bias_Y  <br>
                                             cross_axis     cross_axis        Scale_Z         Bias_Z       */
            float noise_variances[3];
            float bias_variances[3];
        };

        /**
         * @struct intrinsics
         * @brief The stream intrinsic paremeters.
         *
         * The intrinsics parameters describe the relationship between the 2D and 3D coordinate systems of the camera stream.
         * The image produced by the camera is slightly different, depending on the camera distortion model. However, the intrinsics
         * parameters are sufficient to describe the images produced from the different models, using different closed-form formula.
         * The parameters are used for projection operation - mapping points from 3D coordinate space to 2D pixel location in the image,
         * and deprojection operation - mapping 2D pixel, using its depth data, to a point in the 3D coordinate space.
         */
        struct intrinsics
        {
            int             width;     /**< width of the image in pixels                                                                     */
            int             height;    /**< height of the image in pixels                                                                    */
            float           ppx;       /**< horizontal coordinate of the principal point of the image, as a pixel offset from the left edge  */
            float           ppy;       /**< vertical coordinate of the principal point of the image, as a pixel offset from the top edge     */
            float           fx;        /**< focal length of the image plane, as a multiple of pixel width                                    */
            float           fy;        /**< focal length of the image plane, as a multiple of pixel height                                   */
            distortion_type model;     /**< distortion model of the image                                                                    */
            float           coeffs[5]; /**< distortion coefficients                                                                          */
        };

        /**
         * @struct extrinsics
         * @brief The camera extrinsics parameters
         *
         * The extrinsics parameters describe the relationship between different 3D coordinate systems.
         * Camera streams are produced by imagers in different locations. The extrinsics parameters are used to transform 3D points
         * from one camera coordinate system to another camera coordinate system. The transformation is a standard affine transformation
         * using a 3x3 rotation matrix and a 3-component translation vector.
         */
        struct extrinsics
        {
            float rotation[9];    /**< column-major 3x3 rotation matrix          */
            float translation[3]; /**< 3 element translation vector, in meters   */
        };

        /**
          @struct image_info
        * @brief image_info describes is an image detailed information.
        */
        struct image_info
        {
            int32_t                   width;              /**< width of the image in pixels                                          */
            int32_t                   height;             /**< height of the image in pixels                                         */
            pixel_format              format;             /**< image pixel format                                                    */
            int32_t                   pitch;              /**< number of bytes in a single image row of pixels, also known as stride */
        };

        /**
         * @struct pointI32
         */
        struct pointI32
        {
            int32_t x, y; /**< representing a two dimensional point */
        };

        /**
         * @struct pointF32
         */
        struct pointF32
        {
            float x, y; /**< representing a two dimensional point */
        };

        /**
         * @struct point3dF32
         */
        struct point3dF32
        {
            float x, y, z; /**< representing a three-dimensional point */
        };

        /**
         * @struct rect
         */
        struct rect
        {
            int x;
            int y;
            int width;
            int height;
        };

        /**
         * @struct rectF32
         */
        struct rectF32
        {
            float x;
            float y;
            float width;
            float height;
        };

        /**
         * @enum motion_type
         */
        enum class motion_type : int32_t
        {
            accel       = 1,    /**< accelerometer */
            gyro        = 2,    /**< gyroscope     */

            max,
        };

        /**
         * @enum timestamp_domain
         * @brief timestamp_domain is the source of the timestamp.
         */
        enum class timestamp_domain
        {
            camera,
            microcontroller
        };
    }
}
