/*=========================================================================

Library:   TubeTK/VTree

Authors: Stephen Aylward, Julien Jomier, and Elizabeth Bullitt

Original implementation:
Copyright University of North Carolina, Chapel Hill, NC, USA.

Revised implementation:
Copyright Kitware Inc., Carrboro, NC, USA.

All rights reserved. 

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __itkRadiusExtractor_h
#define __itkRadiusExtractor_h

#include <vector>

#include <itkVesselTubeSpatialObject.h>

#include "itkOptParabolicFit1D.h"
#include "itkBlurImageFunction.h"

namespace itk 
{

/**
 * This class extract the radius of a tube given an image
 * 
 * /sa itkRidgeExtractor
 */

template <class TInputImage>             
class ITK_EXPORT RadiusExtractor : public Object 
{
public:

  /** 
   * Standard self typedef */
  typedef RadiusExtractor                                    Self;
  typedef Object                                             Superclass;
  typedef SmartPointer<Self>                                 Pointer;
  typedef SmartPointer<const Self>                           ConstPointer;

  itkTypeMacro( RadiusExtractor, Object );
  itkNewMacro( RadiusExtractor );

  typedef VesselTubeSpatialObject< TInputImage::ImageDimension > 
                                                             TubeType;
  typedef typename TubeType::TubePointType                   TubePointType;
  typedef typename TubeType::PointType                       PointType;

  /**
   * Type definition for the input image. */
  typedef TInputImage                                        ImageType;

  /**
   * Type definition for the input image pixel type. */
  typedef typename TInputImage::PixelType                    PixelType;

  /**
   * Defines the type of vectors used
   */
  typedef Vector<double, 3>                                  VectorType; 

  /**
   * Standard for the number of dimension
   */
  itkStaticConstMacro( ImageDimension, unsigned int, 
    ::itk::GetImageDimension< TInputImage>::ImageDimension );

  /**
   * Set the input image */
  void SetInputImage( typename ImageType::Pointer inputImage );

  /**
   * Get the input image */
  itkGetConstObjectMacro( Image, ImageType );

  /**
   * Set Scale */
  void SetScale( double scale );

  /**
   * Get Scale */
  itkGetMacro( Scale, double );

  /**
   * Set Extent */
  void SetExtent( double extent );

  /**
   * Get Extent */
  itkGetMacro( Extent, double );

  /*  
   * Set Data Minimum */
  itkSetMacro( DataMin, double ); 

  /**
   * Get Data Minimum */
  itkGetMacro( DataMin, double );

  /*  
   * Set Data Maximum */
  itkSetMacro( DataMax, double ); 

  /**
   * Get Data Maximum */
  itkGetMacro( DataMax, double );

  /**
   * Set Minimum Radius */
  void SetRadiusMin( double radiusMin );

  /**
   * Get Minimum Radius */
  itkGetMacro( RadiusMin, double );  

  /**
   * Set Maximum Radius */
  void SetRadiusMax( double radiusMax );

  /**
   * Get Maximum Radius */
  itkGetMacro( RadiusMax, double );

  /**
   * Set Radius0 */
  itkSetMacro( Radius0, double );

  /**
   * Get Radius0 */
  itkGetMacro( Radius0, double ); 

  /**
   * Set ThreshMedialness */
  itkSetMacro( ThreshMedialness, double );

  /**
   * Get ThreshMedialness */
  itkGetMacro( ThreshMedialness, double ); 

  /**
   * Set ThreshMedialness Start */
  itkSetMacro( ThreshMedialnessStart, double );

  /**
   * Get ThreshMedialness Start*/
  itkGetMacro( ThreshMedialnessStart, double ); 

  /**
   * Set Extract Ridge */
  itkSetMacro( ExtractRidge, bool );

  /**
   * Get ExtractRidge*/
  itkGetMacro( ExtractRidge, bool ); 

  /**
   * Return the optimizer */
  OptParabolicFit1D & GetMedialnessOpt( void );

  /**
   * Compute Medialness and Branchness */    
  void ComputeMnessBness( double pntR, double w, 
    double *kernPos, double *kernPosCnt, 
    double *kernNeg, double *kernNegCnt, 
    double *kernBrn, double *kernBrnCnt, 
    double &mness, double &bness, bool doBNess );

  /**
   * Compute the medialness at a point */    
  double MedialnessAtPoint( TubePointType pnt, double pntR, 
    bool doBNess=false, bool newKern=true, double w=1 );

  /**
   * Compute the medialness at a kernel */       
  double MedialnessAtKern( std::list<TubePointType> * tube, double pntR,
    bool doBNess );

  /**
   * Calculate the optimal scale */    
  bool CalcOptimalScale( TubePointType pnt, bool firstGuess =false );

  /**
   * Calculate Radii one way */    
  bool CalcRadiiOneWay( typename std::vector<TubePointType>::iterator
    tubePntFrom, typename std::vector<TubePointType>::iterator tubePntTo,
    bool forward=true );
  /**
   * Calculate Radii */    
  bool CalcRadii( TubeType * tube );
       
  void SetIdleCallBack( bool ( *idleCallBack )() );
  void SetStatusCallBack( void ( *statusCallBack )( char *, char *, int ) );

protected:

  RadiusExtractor();
  virtual ~RadiusExtractor();
  RadiusExtractor( const Self& ) {}
  void operator=( const Self& ) {}

private:

  typename ImageType::Pointer             m_Image; 

  typename BlurImageFunction<ImageType>::Pointer
                                          m_DataOp;
  OptParabolicFit1D                       m_MedialnessOpt;
  
  bool                                    m_Debug;
  bool                                    m_Verbose;
  
  double                                  m_DataMin;
  double                                  m_DataMax;
       
  int                                     m_NumRadiusPoints;
  int                                     m_RadiusPointSpacing;
       
  /** Determine if the algorithm extracts ridge or a valley */
  bool                                    m_ExtractRidge;

  double                                  m_Radius0;
  double                                  m_RadiusMin;
  double                                  m_RadiusMax;
       
  double                                  m_ThreshMedialness;
  double                                  m_ThreshMedialnessStart;
      
  TubePointType                         * m_KernPntArray;
  typename std::vector<TubePointType>::iterator  
                                        * m_IterPntArray;
  int                                     m_ArrayLen;

  std::list<TubePointType>                m_Kern;
  
  double                                  m_KernMedial;
  double                                  m_KernBranch;
  UserFunc<double, double> *              m_MedialnessAtKern;
      
  int                                     m_KernNumT;
  double                                  m_KernCosT[20];
  double                                  m_KernSinT[20];
  double                                  m_KernPos[40];
  double                                  m_KernNeg[40];
  double                                  m_KernPosCnt[40];
  double                                  m_KernNegCnt[40];
  double                                  m_KernBrn[40];
  double                                  m_KernBrnCnt[40];
  VectorType                              m_KernN0;
  VectorType                              m_KernN1;

  int                                     m_TubePointCount;
  int                                     m_TubeLength;

  double                                  m_Scale;
  double                                  m_Extent;   

  bool ( *m_IdleCallBack )();
  void ( *m_StatusCallBack )( char *, char *, int );

  void CalcKernArray( TubeType * tube );
  void CalcKernRadiiOneWay( int iStart, int iEnd, bool forward );
  void CalcKernMeasures( void );
  void ApplyKernMeasures( TubeType * tube );

};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRadiusExtractor.txx"
#endif

#endif /* __itkRadiusExtractor_h */


