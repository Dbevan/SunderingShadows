//
//  _update.c
//  
//  Tlaloc's attempt at rewriting the update command with
//  new functionality.
//  
//  Combines update and dirupdate into one file.
//  "update *" reloads all files in current directory
//  
//  FLAGS as FOLLOWS
//  -r - ignore inherited files
//  -R - update all inherited files
//  -z - update only inherited files "older" than the target
//  
//  -- Tlaloc -- 10/27/21
//

#include <std.h>
#include <security.h>

inherit DAEMON;

int cmd_update(string str)
{
    string file, *files, tmp, *flags, *input;
    int deep, dirupdate;
    object ob, *obs;
    
    if(str == "*")
    {
        file = resolv_path(this_player()->get_path(), "");
        files = get_dir(file + "/*.c");
        
        if(!sizeof(files))
        {
            write("No files in current directory.");
            return 1;
        }
        
        foreach(string temp in files)
        {
            if(file_size(file + "/" + temp) == -1)
                continue;
            
            do_update(file + "/" + temp, 0);
        }
        
        return 1;
    }

    if(!strlen(str) || str == "here")
        file = file_name(environment(this_player()));
    else
    {
        input = explode(str, " ");         
        file = resolv_path(this_player()->get_path(), input[0]);
    }
    //tell_object(this_player(), "file = " + file);
    
    if(sizeof(input) > 1)
        input = input[1..];
    else
        input = ({  });
    
    if(sizeof(input))
    {
        if(member_array("-r", input) >= 0)
            deep = 1;
        if(member_array("-R", input) >= 0)
            deep = 2;
        if(member_array("-z", input) >= 0)
            deep = 3;          
    }
    
    /*
    if(!file || file == "here")
        file = "/" + file_name(environment(this_player()));
    */
    
    if(file_size(file) == -2 && file_size(file + ".c") < 0)
    {
        write("Update : file is a directory.\n");
        return 1;
    }
    
    if(file[<2..<1] != ".c")
        file += ".c";

    seteuid(getuid(this_player()));
    
    if(file_size(file) == -1)
    {
        write("File does not exist.");
        return 1;
    }
    
    if(do_update(file, deep) < time())
        write(file + " : No update necessary.");
    
    seteuid(UID_SYSTEM);
        
    return 1;
}

int do_update(string file, int deep)
{
    mixed *info;
    object ob, *obs;
    int tmp, last_inherit;
    
    last_inherit = 0;
    
    obs = ({  });

    if(ob = find_object(file))
    {
        obs = filter_array(all_inventory(ob), (: userp($1) :));
        foreach(object obj in obs)
            obj->move("/d/shadowgate/void");
    }
    
    if(deep == 3)
    {
        info = stat(file);
        ob = load_object(file);
    
        foreach(string fn in inherit_list(ob))
        {
            tmp = do_update(fn, deep > 1 ? deep : 0);
            last_inherit = tmp > last_inherit ? tmp : last_inherit;
        }
    
        if(sizeof(info))
        {
            last_inherit = info[1] > last_inherit ? info[1] : last_inherit;
            //If object is not out of date
            if(info[2] > last_inherit)
                return info[2];
        }
        
        destruct(ob, 1);
    }
    
    else
    {
        ob = find_object(file);
        
        if(ob)
        {
            if(deep)
            {
                foreach(string fn in inherit_list(ob))
                    do_update(fn, deep > 1 ? deep : 0);
            }
                
            destruct(ob, 1);
        }
    }
    
    if(!catch(ob = load_object(file)))
        write(file + "%^GREEN%^BOLD%^ : updated and loaded.%^RESET%^");
    else
    {
        write(file + " : ERROR in loading file.");
        write("%^YELLOW%^BOLD%^" + catch(load_object(file)) + "%^RESET%^");
    }
        
    if(file[0] != '/')
        file = "/" + file;
    
    objectp(ob) && function_exists("reset", ob) && ob->reset();
    
    foreach(object obj in obs)
        ob && obj->move(ob);
    
    return time();
}

int help()
{
    write("Command : update");
    write("Syntax  : update <filename> <flags> OR update *\n");
    write("Updates the designated file or all files in the current directory (using *).");
    write("Several flags can be used with this command, as follows: \n" +
          " '-r' ignores all inherited files\n" +
          " '-R' reloads all inherited files before loading the designated file\n" +
          " '-z' reloads ONLY inherited files that are 'older' than the designated file\n");
    write("Note : flags do not work in conjunction with the wildcard (*).");
    
    return 1;
}
          
    

   