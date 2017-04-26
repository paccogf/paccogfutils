    class OpenFolderDialog
    {
        static private string mockupstr = @"[Folder Selection]";
        private OpenFileDialog dialog;

        private string _FileName;
        private string _FolderName;

        public string FileName
        { 
            get { return _FileName; } 
        }

        public string FolderName
        {
            get { return _FolderName; }
        }

        public OpenFolderDialog()
        {
            dialog = new OpenFileDialog();

            dialog.ValidateNames = false;
            dialog.CheckFileExists = false;
            dialog.CheckPathExists = true;
            dialog.Multiselect = false;
        }

        ~OpenFolderDialog()
        {
            dialog.Dispose();
        }

        public DialogResult ShowDialog()
        {
            dialog.FileName = mockupstr;

            DialogResult result = dialog.ShowDialog();
            
            if (result == DialogResult.OK)
            {
                int i = dialog.FileName.LastIndexOf('\\');

                _FolderName = dialog.FileName.Substring(0, i+1);
                _FileName = dialog.FileName.Substring(i + 1);
                
                if (_FileName == mockupstr)
                {
                    _FileName = "";
                }
            }

            return result;
        }
    }