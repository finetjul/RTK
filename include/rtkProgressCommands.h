/*=========================================================================
 *
 *  Copyright RTK Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef rtkProgressCommands_h
#define rtkProgressCommands_h

namespace rtk
{

/** \class ProgressCommand
 * \brief Abstract superclass to all progress callbacks.
 * Derived classes must implement the Run() method.
 *
 * \author Aurélien Coussat
 *
 * \ingroup RTK
 *
 */
template <typename TCaller>
class ProgressCommand : public itk::Command
{
protected:
  /** Callback function to be redefined by derived classes. */
  virtual void
  Run(const TCaller * caller) = 0;

public:
  /** Standard class typedefs. */
  typedef ProgressCommand         Self;
  typedef itk::Command            Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  void
  Execute(itk::Object * caller, const itk::EventObject & event) override
  {
    Execute((const itk::Object *)caller, event);
  }

  void
  Execute(const itk::Object * caller, const itk::EventObject & event) override
  {
    if (!itk::ProgressEvent().CheckEvent(&event))
    {
      return;
    }
    const auto * cCaller = dynamic_cast<const TCaller *>(caller);
    if (cCaller)
    {
      Run(cCaller);
    } // TODO fail when cast fails
  }
};

/** \class PercentageProgressCommand
 * \brief Outputs every time a filter progresses by at least one percent.
 *
 * \author Aurélien Coussat
 *
 * \ingroup RTK
 *
 */
template <typename TCaller>
class PercentageProgressCommand : public ProgressCommand<TCaller>
{
public:
  /** Standard class typedefs. */
  typedef PercentageProgressCommand Self;
  typedef ProgressCommand<TCaller>  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  itkNewMacro(Self);

  int percentage = -1;

protected:
  void
  Run(const TCaller * caller) override
  {
    int newPercentage = (int)(caller->GetProgress() * 100.);
    if (newPercentage > percentage)
    {
      std::cout << caller->GetNameOfClass() << " " << newPercentage << "% completed."
                << std::endl; // TODO allow string personnalization
      percentage = newPercentage;
    }
  }
};

} // end namespace rtk

#endif
