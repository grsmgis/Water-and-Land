import os



fp = open("required_info.txt", "w+")
# assign directory
starting_dir = "../outputvars"
for folder in os.listdir(starting_dir):
    important_files = {"soilmoisture1":[], "soilmoisture2":[], "soilmoisture3":[], "soilmoisture4":[], "interflowarea":[], "overlandflowarea":[], "streamflowarea":[] }  
    rootdir = folder
    for subdir, dirs, files in os.walk(starting_dir+ '/' + rootdir):
        for file in files:
            if file in important_files.keys():
                important_files[file].append(os.path.join(subdir, file))
    main = folder.split("_")[0][3:]
    print(main)
    print("Main:",main, file=fp)
    print("total_variables:", file=fp)
    for variables in important_files.keys():
        print(variables, file=fp)
    print("end_of_total_variables", file=fp)
    for key, value in important_files.items():
        value.sort()
        print("var: " + key, file=fp)
        for val in value:
            print(val, file=fp)
    print("end_of_main", file=fp)
    important_files.clear()
    break
fp.close()