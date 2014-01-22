//
//  dimention.cpp
//  Solver
//
//  Created by 李博 on 14-1-22.
//  Copyright (c) 2014年 Libo. All rights reserved.
//

#ifndef __Solver__dimension__
#define __Solver__dimension__

#endif /* defined(__Solver__dimention__) */

#include <iostream>
#include <string>
#include <list>
#include <map>
#include "mxml.h"

namespace UnitConfigure
{
    class dimensionNode
    {
    private:
        std::string name;
    public:
        std::list<float> dimension;
        float coefficient;
        float offset;
        dimensionNode()
        {
        }
        void setDimensionName(std::string name)
        {
            this->name = name;
        }
        void addDimension(float dimension)
        {
            this->dimension.push_back(dimension);
        }
        void addCoefficientAndOffset(float coefficient, float offset)
        {
            this->coefficient = coefficient;
            this->offset = offset;
        }
    };//end of class dimensionNode
    class dimension
    {
    private:
        std::string xmlPath;
        //量纲配置文件 所有单位表示
        std::map<std::string, dimensionNode> resultTable;
    
    private:
        int getBaseDimension();//读取xml基础量纲
        dimensionNode parseExtDimension(std::string define);
        int getExtDimension();//读取xml扩展量纲，有待完善
        
    public:
        dimension(std::string path)
        {
            xmlPath = path;
            getBaseDimension();
            getExtDimension();
        }
        dimensionNode getDimension(std::string dimName);//使用该方法获取量纲节点
        std::list<float> parseSplit(std::string parseString);
    };
    int dimension::getBaseDimension()//读取xml基础量纲
    {
        FILE *fp;
        mxml_node_t *tree;
        
        fp = fopen("filename.xml", "r");
        tree = mxmlLoadFile(NULL, fp,
                            MXML_TEXT_CALLBACK);
        fclose(fp);
        
        TiXmlElement* rootElement = myDocument->;
        XmlNodeList baseNodeList = dimensionXMl.SelectNodes("/dimension/base/node");
        if (baseNodeList != null)
        {
            for (int i = 0; i < baseNodeList.Count; i++)//遍历基础量纲节点
            {
                string name = baseNodeList[i].Attributes["name"].Value;
                //所查找内容为基础量纲
                
                dimensionNode dimNode = new dimensionNode();
                dimNode.setDimensionName(name);
                
                Console.WriteLine("初始化基础量纲:" + name);
                
                for (int j = 0; j < baseNodeList.Count; j++)//生成量纲节点
                {
                    if (j == i)
                    {
                        dimNode.addDimension(1);
                    }
                    else
                    {
                        dimNode.addDimension(0);
                    }
                    dimNode.addCoefficientAndOffset(1, 0);
                }
                this.resultTable.Add(name, dimNode);
                // return dimNode;
                
                //所查找内容可能为基础子量纲
                
                XmlNodeList baseChildList = baseNodeList[i].ChildNodes;
                if (baseChildList != null)
                {
                    for (int k = 0; k < baseChildList.Count; k++)
                    {
                        string nameChild = baseChildList[k].Attributes["name"].Value;
                        //所查找内容为基础子量纲
                        
                        dimensionNode dimNode2 = new dimensionNode();
                        dimNode2.setDimensionName(nameChild);
                        
                        Console.WriteLine("初始化基础子量纲:" + nameChild);
                        
                        for (int l = 0; l < baseNodeList.Count; l++)
                        {
                            if (l == i)
                            {
                                float coefficient = float.Parse(baseChildList[k].Attributes["coefficient"].Value);
                                float offset = float.Parse(baseChildList[k].Attributes["offset"].Value);
                                dimNode2.addCoefficientAndOffset(coefficient, offset);
                                dimNode2.addDimension(1);
                            }
                            else
                            {
                                dimNode2.addDimension(0);
                            }
                        }
                        this.resultTable.Add(nameChild, dimNode2);
                        //return dimNode;
                            
                    }
                }
            }
        }
        return 0;
    }
    dimensionNode dimension::getDimension(std::string dimName)//使用该方法获取量纲节点
    {
        if (this.resultTable.ContainsKey(dimName))
        {
            //Console.WriteLine("该记录从hash表中取得");
            return (dimensionNode)this.resultTable[dimName];
        }
        else
        {
            Console.WriteLine("量纲未查找到！");
            return null;
        }
    }

    int dimension::getExtDimension()//读取xml扩展量纲，有待完善
    {
        XmlDocument dimensionXMl = new XmlDocument();
        dimensionXMl.Load(xmlPath);
        XmlNodeList extNodeList = dimensionXMl.SelectNodes("/dimension/extends/node");
        if(extNodeList!=null)
        {
            for (int i = 0; i < extNodeList.Count; i++)
            {
                string name = extNodeList[i].Attributes["name"].Value;
                string descript = extNodeList[i].Attributes["descript"].Value;
                Console.WriteLine("初始化扩展量纲:" + name + "具体信息为:" + descript);
                this.resultTable.Add(name, parseExtDimension(descript));
                
            }
        }
        return 0;
    }
    dimensionNode dimension::parseExtDimension(std::string define)
    {
        dimensionOperator myOperator=new dimensionOperator();
        dimensionNode tmpNode=null;
        String left = "";
        String right = "";
        if (define.Length >= 1)
        {
            ArrayList result= parseSplit(define);
            left=(String)result[1];
            right = (String)result[2];
            tmpNode=getDimension(right);
            
            if(tmpNode==null)
            {
                return null;
            }
            switch((String)result[0])
            {
                case "":
                    return tmpNode;
                case "/":
                    return myOperator.div(parseExtDimension(left),tmpNode);
                case "*":
                    return myOperator.mul( parseExtDimension(left),tmpNode);
                default: return null;
            }
        }
        return null;
    }
    
    ArrayList dimenison::parseSplit(String parseString)
    {
        //            Console.WriteLine(parseString);
        ArrayList result = new ArrayList();
        String res = "";
        String tmp = parseString.Substring(parseString.Length-1, 1);
        while (tmp != "/" && tmp != "*" && parseString.Length >= 1)
        {
            res = tmp+res;
            parseString = parseString.Substring(0, parseString.Length - 1);
            if (parseString.Length != 0)
            {
                tmp = parseString.Substring(parseString.Length-1, 1);
            }
            else
            {
                tmp = "";
            }
        }
        result.Add(tmp);
        if (parseString.Length < 1)
        {
            result.Add("");
        }
        else
        {
            result.Add(parseString.Substring(0, parseString.Length - 1));
        }
        result.Add(res);
        
        
        //            Console.WriteLine("operator:"+result[0]+"left:"+result[1]+" right:"+result[2]);
        //            Console.WriteLine();
        return result;
    }
}
