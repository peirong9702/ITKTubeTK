/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAnisotropicCoherenceEnhancingDiffusionImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2012/03/19
  Version:   $Revision: 1.0 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkEuler3DTransform.h"
#include "itkImageToTubeRigidMetric.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkSpatialObjectToImageFilter.h"
#include "itkSpatialObjectReader.h"
#include "itkTubeSpatialObjectPoint.h"

/**
 *  This test exercised the metric evaluation methods in the
 *  itkImageToTubeRigidMetric class. The distance between
 *  a 3D binary images (32x32x32) and a .tre image is computed and check with
 *  the reference for the metric.
 */

int itkImageToTubeRigidMetricTest(int argc, char* argv [] )
{
  if ( argc < 4 )
    {
    std::cerr << "Missing Parameters: "
              << argv[0]
              << " Input_FixedImage "
              << "Input_SpatialObject "
              << "Input_ExpectedValue."
              << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<double, 3>                             Image3DType;
  typedef itk::ImageRegionIteratorWithIndex< Image3DType >  Image3DIteratorType;
  typedef itk::TubeSpatialObject<3>                         TubeType;
  typedef itk::TubeSpatialObjectPoint<3>                    TubePointType;
  typedef itk::GroupSpatialObject<3>                        TubeNetType;

  typedef itk::ImageFileReader<Image3DType>                 ImageReaderType;
  typedef itk::SpatialObjectReader<3>                       TubeNetReaderType;

  typedef itk::ImageToTubeRigidMetric<Image3DType, TubeNetType>   MetricType;
  typedef itk::Array<double>                                      ParametersType;
  typedef MetricType::InterpolatorType                            InterpolatorType;
  typedef MetricType::TransformType                               TransformType;

  const double epsilonReg = 0.05; // Delta threshold on the measure checking.

  // read image (fixedImage)
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName( argv[1] );
  try
    {
    imageReader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }

  // read tube (spatialObject)
  TubeNetReaderType::Pointer tubeReader = TubeNetReaderType::New();
  tubeReader->SetFileName( argv[2] );
  try
    {
    tubeReader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }

  //------------------------------------------------------------------
  // Compute the metric for a 3D image susampled at 1/30
  //------------------------------------------------------------------
  MetricType::Pointer metric = MetricType::New();
  metric->SetExtent( 3 );
  metric->SetSampling( 30 );
  metric->SetVerbose( true );

  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  TransformType::Pointer transform = TransformType::New();
  TransformType::ParametersType parameters = transform->GetParameters();

  metric->SetFixedImage( imageReader->GetOutput() );
  metric->SetMovingSpatialObject ( tubeReader->GetGroup() );
  metric->SetInterpolator( interpolator );
  metric->SetTransform( transform );
  try
    {
    metric->Initialize();
    }
  catch ( itk::ExceptionObject &excp )
    {
    std::cerr << "Exception caught while initializing metric." << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  MetricType::MeasureType value = metric->GetValue( parameters );
  if (value < ( atof(argv[3]) - epsilonReg ) ||
      value > ( atof(argv[3]) + epsilonReg ) )
    {
    std::cerr << "Distance value different than expected."
              << value
              << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}