// -*- LSST-C++ -*-
#if !defined(LSST_DEBLENDER_BASELINE_H)
#define LSST_DEBLENDER_BASELINE_H
//!

#include <vector>
#include <utility>

#include "lsst/afw/image/Image.h"
#include "lsst/afw/image/MaskedImage.h"
#include "lsst/afw/detection/Footprint.h"
#include "lsst/afw/detection/HeavyFootprint.h"
#include "lsst/afw/detection/Peak.h"

namespace lsst {
    namespace meas {
        namespace deblender {

            template <typename ImagePixelT,
                      typename MaskPixelT=lsst::afw::image::MaskPixel,
                      typename VariancePixelT=lsst::afw::image::VariancePixel>
            class BaselineUtils {

            public:
                typedef typename lsst::afw::image::MaskedImage<ImagePixelT, MaskPixelT, VariancePixelT> MaskedImageT;
                typedef typename std::shared_ptr<lsst::afw::image::MaskedImage<ImagePixelT, MaskPixelT, VariancePixelT>> MaskedImagePtrT;
                typedef typename lsst::afw::image::Image<ImagePixelT> ImageT;
                typedef typename std::shared_ptr<lsst::afw::image::Image<ImagePixelT>> ImagePtrT;
                typedef typename lsst::afw::image::Mask<MaskPixelT> MaskT;
                typedef typename std::shared_ptr<lsst::afw::image::Mask<MaskPixelT>> MaskPtrT;

                typedef typename lsst::afw::detection::Footprint FootprintT;
                typedef typename std::shared_ptr<lsst::afw::detection::Footprint> FootprintPtrT;
                typedef typename lsst::afw::detection::HeavyFootprint<ImagePixelT, MaskPixelT, VariancePixelT> HeavyFootprintT;

                typedef typename std::shared_ptr<lsst::afw::detection::HeavyFootprint<ImagePixelT, MaskPixelT, VariancePixelT>> HeavyFootprintPtrT;

                static
                std::shared_ptr<lsst::afw::detection::Footprint>
                symmetrizeFootprint(lsst::afw::detection::Footprint const& foot,
                                    int cx, int cy);

                static
                std::pair<ImagePtrT, FootprintPtrT>
                buildSymmetricTemplate(MaskedImageT const& img,
                                       lsst::afw::detection::Footprint const& foot,
                                       lsst::afw::detection::PeakRecord const& pk,
                                       double sigma1,
                                       bool minZero,
                                       bool patchEdges,
                                       bool* patchedEdges);

                static void
                medianFilter(ImageT const& img,
                             ImageT & outimg,
                             int halfsize);

                static void
                makeMonotonic(ImageT & img,
                              lsst::afw::detection::PeakRecord const& pk);

                static const int ASSIGN_STRAYFLUX                          = 0x1;
                static const int STRAYFLUX_TO_POINT_SOURCES_WHEN_NECESSARY = 0x2;
                static const int STRAYFLUX_TO_POINT_SOURCES_ALWAYS         = 0x4;
                // split flux according to the closest distance to the template?
                // (default is according to distance to the peak)
                static const int STRAYFLUX_R_TO_FOOTPRINT                  = 0x8;
                static const int STRAYFLUX_NEAREST_FOOTPRINT              = 0x10;
                static const int STRAYFLUX_TRIM                           = 0x20;

                // swig doesn't seem to understand std::vector<MaskedImagePtrT>...
                static
                std::vector<typename std::shared_ptr<lsst::afw::image::MaskedImage<ImagePixelT, MaskPixelT, VariancePixelT>>>
                apportionFlux(MaskedImageT const& img,
                              lsst::afw::detection::Footprint const& foot,
                              std::vector<typename std::shared_ptr<lsst::afw::image::Image<ImagePixelT>>> templates,
                              std::vector<std::shared_ptr<lsst::afw::detection::Footprint> > templ_footprints,
                              //
                              ImagePtrT templ_sum,
                              std::vector<bool> const& ispsf,
                              std::vector<int>  const& pkx,
                              std::vector<int>  const& pky,
                              std::vector<std::shared_ptr<typename lsst::afw::detection::HeavyFootprint<ImagePixelT,MaskPixelT,VariancePixelT> > > & strays,
                              int strayFluxOptions,
                              double clipStrayFluxFraction
                     );

                static
                bool
                hasSignificantFluxAtEdge(ImagePtrT,
                                         std::shared_ptr<lsst::afw::detection::Footprint>,
                    ImagePixelT threshold);

                static
                std::shared_ptr<lsst::afw::detection::Footprint>
                getSignificantEdgePixels(ImagePtrT,
                                         std::shared_ptr<lsst::afw::detection::Footprint>,
                                         ImagePixelT threshold);


                static
                void
                _sum_templates(std::vector<ImagePtrT> timgs,
                               ImagePtrT tsum);

                static
                void
                _find_stray_flux(lsst::afw::detection::Footprint const& foot,
                             ImagePtrT tsum,
                             MaskedImageT const& img,
                             int strayFluxOptions,
                             std::vector<std::shared_ptr<lsst::afw::detection::Footprint> > tfoots,
                             std::vector<bool> const& ispsf,
                             std::vector<int>  const& pkx,
                             std::vector<int>  const& pky,
                             double clipStrayFluxFraction,
                             std::vector<std::shared_ptr<typename lsst::afw::detection::HeavyFootprint<ImagePixelT,MaskPixelT,VariancePixelT> > > & strays);

            };
        }
    }
}

#endif
