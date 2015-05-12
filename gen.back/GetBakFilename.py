def GetBakFilename(filename):

    bakext = '.bak'
    if not os.path.isfile(filename):
        raise Exception("%s: no existe el fichero" % filename)

    if not os.path.exists(filename + bakext):
        return filename + bakext
    else:
        i = 0
        while i > -1:
            newpath = "%s%s%d" % (filename, bakext, i)
            if not os.path.exists(newpath):
                return filename, newpath
            i += 1
