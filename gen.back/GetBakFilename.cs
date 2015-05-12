        /// <summary>
        /// OBTENER NUEVO NOMBRE DE FICHERO .BAKx A PARTIR DE filename. 
        /// SE PUEDE CAMBIAR LA EXTENSION O ANULARLA PASANDO LA CADENA VACIA "" COMO PARAMETRO
        /// dotext NO PUEDE SER null!!!
        /// </summary>
        /// <param name="filename"></param>
        /// <param name="dotext"></param>
        /// <returns></returns>
        static string GetBakFilename(string filename, string dotext = ".bak")
        {
            string retval = filename;

            // NI SIQUIERA EXISTE FICHERO --> NOS VALE EL NOMBRE
            if (!File.Exists(retval))
            {
                return retval;
            }


            // MIRAR A VE SI EXISTE CON EXTENSION DADA (PODEMOS ANULAR EL AÑADIR EXTENSION)
            if (dotext != "")
            {
                retval = retval + dotext;
                if (!File.Exists(retval))
                {
                    return retval;
                }
            }
           
            // ENCONTRAR SIGUIENTE filename+dotext+numero
            // HASTA QUE OCURRA UN INTEGER OVERFLOW E i PASE A NEGATIVO (MUCHOS ENTEROS HASTA QUE ESTO PASE)
            int i = 0;
            do
            {
                string aux = String.Format("{0}{1}", retval, i);

                if (!File.Exists(aux))
                {
                    retval = aux;
                    break;
                }

                i++;
            }
            while (!(i < 0));
            
            return retval;
        }
