/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __itkTubeBasisFeatureVectorGenerator_h
#define __itkTubeBasisFeatureVectorGenerator_h

#include <vector>

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"

#include "itkImage.h"

#include "itkTubeFeatureVectorGenerator.h"

namespace itk
{

namespace tube
{

template< class ImageT, class LabelmapT >
class BasisFeatureVectorGenerator
: public FeatureVectorGenerator< ImageT >
{
public:

  typedef BasisFeatureVectorGenerator            Self;
  typedef FeatureVectorGenerator< ImageT >       Superclass;
  typedef SmartPointer< Self >                   Pointer;
  typedef SmartPointer< const Self >             ConstPointer;

  itkTypeMacro( BasisFeatureVectorGenerator, FeatureVectorGenerator );

  itkNewMacro( Self );

  //
  // Custom Typedefs
  //
  typedef typename Superclass::ImageType         ImageType;

  typedef LabelmapT                              LabelmapType;

  itkStaticConstMacro( ImageDimension, unsigned int,
    ImageT::ImageDimension );

  typedef typename Superclass::IndexType         IndexType;

  typedef typename Superclass::FeatureValueType  FeatureValueType;
  typedef typename Superclass::FeatureVectorType FeatureVectorType;
  typedef typename Superclass::FeatureImageType  FeatureImageType;

  typedef FeatureVectorGenerator< ImageT >       FeatureVectorGeneratorType;

  typedef typename LabelmapT::PixelType          ObjectIdType;
  typedef std::vector< ObjectIdType >            ObjectIdListType;

  typedef typename Superclass::ValueType         ValueType;
  typedef typename Superclass::ValueListType     ValueListType;

  typedef vnl_vector< ValueType >                VectorType;
  typedef std::vector< VectorType >              VectorListType;

  typedef vnl_matrix< ValueType >                MatrixType;
  typedef std::vector< MatrixType >              MatrixListType;

  //
  // Methods
  //
  void         SetInputFeatureVectorGenerator( FeatureVectorGeneratorType
                 * fGen );
  typename FeatureVectorGeneratorType::Pointer GetInputFeatureVectorGenerator(
                 void );

  itkSetObjectMacro( Labelmap, LabelmapType );
  itkGetObjectMacro( Labelmap, LabelmapType );

  void         SetObjectId( ObjectIdType objectId );
  void         AddObjectId( ObjectIdType objectId );
  ObjectIdType GetObjectId( unsigned int num = 0 ) const;
  unsigned int GetNumberOfObjectIds( void ) const;

  ValueType       GetObjectMean( ObjectIdType objectId ) const;
  void            SetObjectMean( ObjectIdType objectId, ValueType val );
  MatrixType      GetObjectCovariance( ObjectIdType objectId ) const;
  void            SetObjectCovariance( ObjectIdType objectId,
                    MatrixType val );

  ValueType       GetGlobalMean( void ) const;
  void            SetGlobalMean( ValueType val );
  MatrixType      GetGlobalCovariance( void ) const;
  void            SetGlobalCovariance( MatrixType val );

  unsigned int    GetNumberOfBasis( void ) const;

  double          GetBasisValue( unsigned int basisNum ) const;
  VectorType      GetBasisVector( unsigned int basisNum ) const;
  MatrixType      GetBasisMatrix( void ) const;
  VectorType      GetBasisValues( void ) const;

  void            SetBasisValue( unsigned int basisNum, double value );
  void            SetBasisVector( unsigned int basisNum,
                    const VectorType & vec );
  void            SetBasisMatrix( const MatrixType & mat );
  void            SetBasisValues( const VectorType & values );

  typename FeatureImageType::Pointer GetFeatureImage( unsigned int fNum )
                    const;

  itkSetMacro( PerformLDA, bool );
  itkGetMacro( PerformLDA, bool );
  itkSetMacro( PerformPCA, bool );
  itkGetMacro( PerformPCA, bool );

  virtual void GenerateBasis( void );

  void SetNumberOfBasisToUseAsFeatures( unsigned int numBasisUsed );

  virtual unsigned int      GetNumberOfFeatures( void ) const;

  virtual FeatureVectorType GetFeatureVector( const IndexType & indx )
    const;

  virtual FeatureValueType  GetFeatureVectorValue( const IndexType & indx,
    unsigned int fNum ) const;

protected:

  BasisFeatureVectorGenerator( void );
  virtual ~BasisFeatureVectorGenerator( void );

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  // Purposely not implemented
  BasisFeatureVectorGenerator( const Self & );
  void operator = ( const Self & );      // Purposely not implemented

  //  Data
  typename FeatureVectorGeneratorType::Pointer
                                  m_InputFeatureVectorGenerator;

  typename LabelmapType::Pointer  m_Labelmap;

  ObjectIdListType                m_ObjectIdList;
  VectorListType                  m_ObjectMeanList;
  MatrixListType                  m_ObjectCovarianceList;

  VectorType                      m_GlobalMean;
  MatrixType                      m_GlobalCovariance;

  bool                            m_PerformLDA;
  bool                            m_PerformPCA;

  unsigned int                    m_NumberOfBasis;
  unsigned int                    m_NumberOfBasisToUseAsFeatures;

  MatrixType                      m_BasisMatrix;
  VectorType                      m_BasisValues;
};

}

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTubeBasisFeatureVectorGenerator.txx"
#endif

#endif
