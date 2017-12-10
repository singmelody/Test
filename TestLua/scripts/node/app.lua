require "lfs"

local prefixfilePath = "../../Data/scripts/node/"

package.path = package.path .. prefixfilePath .. "?.lua"

local path = {
	"../../Data/scripts/node/scene",
	"../../Data/scripts/node/packet",
}

function LuaOnStart()
	print("App OnStartUp");
	for i = 1, #path do
		local fileName = path[i];
		print(fileName);
		for file in lfs.dir(fileName) do
			if file ~= "." and file ~= ".." then
				local f = fileName .. '/' .. file