#ifndef __ND_FileDialog_H__
#define __ND_FileDialog_H__

namespace NativeDialog
{
   /*!
     @brief File dialog to select file pathes both open and save mode are supported
     @par Select file(s) and directory(directories)
     @par Selection filters are supported
     @par Platforms: Windows, Mac OSX and *inux(GTK+3.0 is required)
     @attention Select both file and directory mode is not supported on Linux if you set to this mode the dialog only allows folder selection
     @attention Select both file and directory mode is NOT full supported on windows for select multiple file and dirctory mode
     @note The @b decideHandler whould be called when selection confirmed or call the @b cancelHandler when selection canceled
     @see Dialog::EventHandler
    */
  class FileDialog : public Dialog
  {
    using super = Dialog;

    public:
      /// Dialog selection mode
      enum FileDialogMode : unsigned int
      {
        /// Select file mode
        SELECT_FILE     = 1U,
        /// Select directory mode
        SELECT_DIR      = 2U,
        /// Select file and directory mode
        SELECT_FILE_DIR = SELECT_FILE | SELECT_DIR ,
        /// Allows multi selection , Windows does not support multiselect directorys
        MULTI_SELECT    = 4U,
        /// Select save file mode
        SELECT_TO_SAVE  = 8U
      };

      ///Default constructor
      FileDialog()
      {
        m_eventOwner = nullptr;
        m_pHostWnd   = nullptr;
        cleanUp();
      }

      /*!
       @brief  Construct a file selection dialog
       @param title    The title text of the dialog
       @param mode     Selection mode
       @param eventOwner The event owner pointer
       @param pHostWnd The host window pointer
      */
      FileDialog(const string& title,int mode,
                 void* eventOwner = nullptr,
                 void* pHostWnd = nullptr)
      {
          m_eventOwner = eventOwner;
          m_pHostWnd   = pHostWnd;
          m_mode = mode;
          m_title = title;
      }

      ~FileDialog()
      {
          m_eventOwner = nullptr;
          m_pHostWnd = nullptr;
      }

      /// Clean up all configrations and options
      inline FileDialog& cleanUp()
      {
                m_mode        = SELECT_FILE;
                m_title    = nullstr;
                m_defaultPath = nullstr;
                setEventOwner(nullptr);
                setHostWindow(nullptr);
                m_filter.clear();
                return *this;
      }

      /// Show dialog
      virtual void show() override;

      /*!
       @brief Check if is saving file mode
       @return Is save mode
      */
      inline bool saveMode()const{ return m_mode & SELECT_TO_SAVE; }

      /*!
       @brief Set to if is  saving file mode
       @param saveMode Is save mode
       */
      inline FileDialog& setSaveMode( bool saveMode)
      {
          if(saveMode)
              m_mode |= SELECT_TO_SAVE;
          else
              m_mode &= ~SELECT_TO_SAVE;
          return *this;
      }

      /*!
       @brief  Check allows to multi-selection
       @return Allows multi-selection
       */
      inline bool allowsMultipleSelection()const{ return m_mode & MULTI_SELECT; }

      /*!
       @brief  Set to if is allowed to multi-selection
       @param allowded Allows multi-selection
       */
      inline FileDialog& setAllowsMultipleSelection(bool allowded)
      {
          if(allowded)
              m_mode |= MULTI_SELECT;
          else
              m_mode &= ~MULTI_SELECT;
          return *this;
      }


      /*!
       @brief  Check if allows to select file(s)
       @return Select file(s) allowded
       */
      inline bool allowsFileSelection()const { return m_mode & SELECT_FILE; }

      /*!
       @brief  Set if is allowed to select file(s)
       @param allowed Allows select file
       */
      inline FileDialog& setAllowsFileSelection(bool allowed)
      {
          if(allowed)
              m_mode |= SELECT_FILE;
          else
              m_mode &= ~SELECT_FILE;
          return *this;
      }

      /*!
       @brief  Check if allows to select directory(directories)
       @return Allowed to select directory
       */
      inline bool allowsDirectorySelection()const { return m_mode & SELECT_DIR; }

      /*!
       @brief  Set if is allowed to select directory(directories)
       @param allowded Allowded to select directory
       */
      inline FileDialog& setAllowsDirectorySelection(bool allowded )
      {
          if(allowded)
              m_mode |= SELECT_DIR;
          else
              m_mode &= ~SELECT_DIR;
          return *this;
      }

      /*!
       @brief Set default selected path
       @param initDir The default selected path(absolute path)
       */
      inline FileDialog& setDefaultPath(const string& initDir)
      {
        m_defaultPath = initDir;
        return *this;
      }

      /*!
       @brief  Get default selected path
       @return The default selected path(absolute path)
       */
      inline const string& defaultPath()const { return m_defaultPath;}

      /*!
       @brief Add selection filter
       @param label Filter name. Invisiable on Mac OSX
       @param types Fileter's extention name.
              @n Does not contains '.'
              @n '*' to allow all files.for more extentions,seperate with ';' e.g “txt;TXT” to allow select files name ends with '.txt' and '.TXT'
       @attention:
             @li It is not allowed to use '*' to select all files on Mac OSX.
             @li If you have to select all files just leave the filter empty for all platform.
      */
      inline FileDialog& addFilter(const string & label ,const string &types)
      {
          m_filter.push_back(std::pair<string, string>(label,types));
          return *this;
      }

      /// Clear filters that means allow to select all kind of files
      inline FileDialog& clearFilters()
      {
        m_filter.clear();
        return *this;
      }

      /*!
       @brief  Get selected pathes
       @return The vector contains all selected pathes( abusolute pathes )
       */
      inline const vector<string>& selectedPathes()const{ return m_selectedPathes;}

      /*!
       @brief  Get the read only dialog's event owner
       @return The dialog event owner
       */
      inline const void* eventOwner()const{  return m_eventOwner; }

      /*!
       @brief  Get the writable dialog's event owner
       @return The dialog event owner
       */
      inline void* eventOwner(){ return m_eventOwner;}

      /*!
       @brief  Set the dialog's event owner
       @param obj The event owner object
       */
      inline FileDialog& setEventOwner(void* obj)
      {
          m_eventOwner = obj;
          return *this;
      }

      /*!
       @brief  Get the read only host window of the dialog
       @return The host window
       */
      inline const void* hostWindow()const{ return m_pHostWnd; }

      /*!
       @brief  Get the writable host window of the dialog
       @return The host window
       */
      inline void* hostWindow(){return m_pHostWnd;}

      /*!
       @brief Set the host window of the dialog
       @param pWnd The window to host the dialog
       */
      inline FileDialog& setHostWindow(void* pWnd)
      {
          m_pHostWnd = pWnd;
          return *this;
      }

      /**
       *  @brief Set dialog mode
       *
       *  @param mode The mode e.g (SELECT_FILE | SELECT_DIR | MULTI_SELECT) allows to select files and dirctories
       */
      inline FileDialog& setMode(unsigned int mode)
      {
        m_mode = mode;
        return *this;
      }

      /*!
       @brief  Get the dialog selection mode
       @return The selection mode
       */
      inline unsigned int mode()const{ return m_mode;}

    protected:
      /// Default selected path
      string m_defaultPath;
      /// Dialog select mode
      unsigned m_mode;
      /// Filters for select files
      vector<std::pair<string,string> > m_filter;
      /// Selected pathes (absolute pathes)
      std::vector<string> m_selectedPathes;
      /// The event owner
      void * m_eventOwner;
      /// Dialog's host window
      void * m_pHostWnd;
    };
}

#endif //__ND_FileDialog_H__
